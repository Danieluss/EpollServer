#ifndef HTML_PARSER_HPP
#define HTML_PARSER_HPP

#include<bits/stdc++.h>

using namespace std;
#define SIZE(a) ((int)a.size())

struct HTMLParser {
private:
    vector<string> words;
    vector<string> links;
    string title;
    string text;
    string html;
    string url;
    string baseurl;
    int position;
    set<int> spaceCharacters {0x20, 0x9, 0xa, 0xc, 0xff, 0xd};
    inline bool checkSubstring(string s);
    void waitForToken(string token);
    string waitForTokenAndReturnContent(string token);
    void parseComment();
    void addLink(string link);
    string parseTag();
    string waitForTagAndReturnContent(string tag);
    void addWord(string &word); 
    string getBaseUrl(string url);
    void sanitize_html();
public:
    HTMLParser(string html, string url);
    void parse();
    void report();
    string getTitle();
    string getText();
    string getUrl();
    vector<string> getWords();
    vector<string> getLinks();
};

#endif //HTML_PARSER_HPP
