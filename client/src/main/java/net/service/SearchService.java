package net.service;

import net.data.SearchResult;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@Service
public class SearchService {

    @Autowired
    private Socket clientSocket;
    private PrintWriter out;
    private BufferedReader in;
    private Pattern responsePattern;
    private final static String FIELD_SEPARATOR = "<\n>";
    private final static String ROW_SEPARATOR = "<\t\n>";

    @PostConstruct
    public void init() throws IOException {
        responsePattern = Pattern.compile("(?<title>.+) + FIELD_SEPARATOR + (?<url>.+) + ROW_SEPARATOR");
        out = new PrintWriter(clientSocket.getOutputStream(), true);
        in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
    }

    public List<SearchResult> requestSearch(String searchQuery) throws IOException {
        out.println(searchQuery + "\n");
        String response = in.readLine();
        return parseResponse(response);

    }

    private List<SearchResult> parseResponse(String response) {
        System.out.println("Got response: " + response);
        Matcher matcher = responsePattern.matcher(response);
        List<SearchResult> resultList = new LinkedList<>();
        while (matcher.find()) {
            String title = matcher.group("title");
            String url = matcher.group("url");
            resultList.add(new SearchResult(title, url));
        }
        return resultList;
    }

    @PreDestroy
    public void close() throws IOException {
        in.close();
        out.close();
        clientSocket.close();
    }

}
