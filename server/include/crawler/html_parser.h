#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include<bits/stdc++.h>

using namespace std;
#define SIZE(a) ((int)a.size())

struct HTMLParser {
private:
    vector <string> words;
    vector <string> links;
    string title;
    string description;
    string text;
    string html;
    string url;
    string baseurl;
    int position;
    set<int> spaceCharacters{0x20, 0x9, 0xa, 0xc, 0xff, 0xd};
    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> convert;
    set<string> domainExtensions{"pl", "com", "uk", "de", "org", "edu", "io"};

    inline bool checkSubstring(string s);

    void waitForToken(string token);

    string waitForTokenAndReturnContent(string token);

    void parseComment();

    void addLink(string link);

    string parseTag();

    string waitForTagAndReturnContent(string tag);

    void addWord(string &word);

    string getBaseUrl(string url);

    string getDomainExtension(string url);

    void sanitize_html();

    void parseText(string &word);

public:
    HTMLParser(string html, string url);

    void parse();

    void report();

    string getTitle();

    string getText();

    string getUrl();

    string getDescription();

    vector <string> getWords();

    vector <string> getLinks();
};

#endif //HTML_PARSER_H
