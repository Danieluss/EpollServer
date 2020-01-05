#include"crawler/html_parser.hpp"

struct HTMLAttr {
    string name;
    string value;
    HTMLAttr() {}
    HTMLAttr(string name, string value) : name(name), value(value) {}
};

inline bool HTMLParser::checkSubstring(string s) {
    for(int i=0; i < SIZE(s); i++) {
        if(tolower(html[position+i]) != tolower(s[i])) {
            return false;
        } 
    }
    return true;
}

void HTMLParser::waitForToken(string token) {
    while(position < SIZE(html)) {
        if(checkSubstring(token)) {
            position+=SIZE(token);
            return;
        }
        position++;
    }
}

string HTMLParser::waitForTokenAndReturnContent(string token) {
    string res="";
    while(position < SIZE(html)) {
        if(checkSubstring(token)) {
            position+=SIZE(token);
            return res;
        }
        res+=html[position];
        position++;
    }
    return res;
}

void HTMLParser::parseComment() {
    waitForToken("-->");
}

void HTMLParser::addLink(string link) {
    if(SIZE(link) <= 1) {
        return;
    }
    regex r("^//.+");
    regex rel("^/[^/].*");
    if(regex_match(link, r)) {
        link = "https:" + link;
    } else if(regex_match(link, rel)) {
        link = baseurl + link;
    }
    if(link.back() == '/') {
        link.pop_back();
    }
    regex valid("https?://.+");
    if(regex_match(link, valid)) {
        links.push_back(link);
    }
}

string HTMLParser::parseTag() {
    if(checkSubstring("!--")) {
        position+=3;
        parseComment();
        return "-->";
    }
    string tag="";
    if(position < SIZE(html) && html[position] == 0x2f) {
        position++;
        tag = "/";
    }
    while(position < SIZE(html)) {
        char c = html[position];
        if(spaceCharacters.count(c) || c == '>' || c == 0x2f) {
            break;
        }
        tag+=tolower(c);
        position++;
    }
    vector<HTMLAttr> attributes;
    HTMLAttr currentAttr;
    bool afterEquals=false, afterSpace=false;
    while(position < SIZE(html)) {
        char c = html[position++];
        if(spaceCharacters.count(c) || c == 0x2f || c == '>') {
            if(SIZE(currentAttr.name) && SIZE(currentAttr.value)) {
                attributes.push_back(currentAttr);
                currentAttr = HTMLAttr();
                afterSpace = false;
                afterEquals = false;
            }
        }
        if(spaceCharacters.count(c) || c == 0x2f) {
            if(SIZE(currentAttr.name)) {
                afterSpace = true;
            }
            continue;
        } else if(c == '>') {
            break;
        } else if(c == '=') {
            afterEquals = true;
        } else if(c == '\'' || c == '"') {
            currentAttr.value = waitForTokenAndReturnContent(string("") + c);
        } else if(afterEquals) {
            currentAttr.value+= tolower(c);
        } else if(afterSpace) {
            afterSpace = false;
            currentAttr.name = string("") + char(tolower(c));
        } else {
            currentAttr.name+= tolower(c);
        }
    }
    // cerr << tag << "\t";
    // for(HTMLAttr a : attributes) {
    //     cerr << a.name << "=" << a.value << ", ";
    // }
    // cerr << "\n";
    if(tag == "a") {
        for(HTMLAttr a : attributes) {
            if(a.name == "href") {
                addLink(a.value);
            }
        }
    }
    return "<" + tag + ">";
}

string HTMLParser::waitForTagAndReturnContent(string tag) {
    string res="";
    while(position < SIZE(html)) {
        char c = html[position++];
        if(c == '<') {
            string t = parseTag();
            if(t == tag) {
                return res;
            }
        }
        res+=c;
    }
    return res;
}

void HTMLParser::addWord(string &word) {
    if(word != "") {
        words.push_back(word);
    }
    word="";
}

string HTMLParser::getBaseUrl(string url) {
    // cerr << url << "\n";
    regex r("(https?://[A-Za-z\\.]+)/.*");
    smatch m;
    if(regex_match(url, m, r)) {
        return m[1].str();
    }
    return "";
}

void HTMLParser::sanitize_html() {
    for(; position < SIZE(html); position++) {
        if(html[position] == '<') {
            break;
        } 
    }
}

void HTMLParser::parseText(string &word) {
    char c = html[position];
    if(spaceCharacters.count(c) == 0 || (SIZE(text) && spaceCharacters.count(text.back()) == 0)) {
        text+=c;
    }
    if(isalnum(c)) {
        word+=c;
    } else if(c < 0 && position+1 < SIZE(html)) {
        position++;
        char d = html[position];
        string s;
        s.push_back(c);
        s.push_back(d);
        bool converted = true;
        u16string w;
        try {
            w = convert.from_bytes(s);
        } catch(exception &e) {
            converted = false;
        }
        if(converted && iswalpha(w[0])) {
            word+=c;
            word+=html[position];
        } else {
            addWord(word);
        }
        text+=html[position];
    } else {
        addWord(word);
    }
    position++;
}

HTMLParser::HTMLParser(string html, string url) : html(html), url(url) {
    position = 0;
    baseurl = getBaseUrl(url);
    sanitize_html();
    setlocale(LC_ALL, "");
    // cerr << baseurl << "\n";
}

void HTMLParser::parse() {
    string word="";
    while(position < SIZE(html)) {
        if(html[position] != '<') {
            parseText(word);
            continue;
        }
        addWord(word);
        position++;
        string t = parseTag();
        // cerr << t << "\n";
        if(t == "<script>") {
            waitForToken("</script>");
        } else if(t == "<style>") {
            waitForToken("</style>");
        } else if(t == "<title>") {
            title = waitForTagAndReturnContent("</title>");
        }
    }
}

void HTMLParser::report() {
    cerr << title << "\n";
    cerr << url << "\n";
    cerr << SIZE(words) << "\n";
    cerr << SIZE(links) << "\n";
    // cout << text << "\n";
    cout << "Words:\n";
    int counter=0;
    for(string w : words) {
        cout << w << ", ";
        counter++;
        if(counter > 10) {
            break;
        }
    }
    // cout << "\nLinks\n";
    // for(string u : links) {
    //     cout << u << "\n";
    // }
}

string HTMLParser::getTitle() {
    return title;
}

string HTMLParser::getText() {
    return text;
}

string HTMLParser::getUrl() {
    return url;
}

vector<string> HTMLParser::getWords() {
    return words;
}

vector<string> HTMLParser::getLinks() {
    return links;
}