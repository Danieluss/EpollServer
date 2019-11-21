package net.service;

import lombok.extern.slf4j.Slf4j;
import net.data.SearchResult;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageImpl;
import org.springframework.data.domain.PageRequest;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import javax.annotation.Resource;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@Service
@Slf4j
public class SearchService {

    @Resource
    Socket clientSocket;
    private Pattern responsePattern;
    private final static String FIELD_SEPARATOR = ";";
    private final static String ROW_SEPARATOR = "\\n";
    private final static String RESPONSE_PATTERN = "(?<url>.+)" + FIELD_SEPARATOR + "(?<title>.+)";

    @PostConstruct
    public void init() throws IOException {
        responsePattern = Pattern.compile(RESPONSE_PATTERN);
    }

    public List<SearchResult> requestSearch(String searchQuery) throws IOException {
        PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
        BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        log.info("---> " + searchQuery);
        out.println(searchQuery);
        StringBuilder responseBuilder = new StringBuilder();
        String responseLine;
        while ((responseLine = in.readLine()) != null) {
            responseBuilder.append(responseLine);
            responseBuilder.append('\n');
        }
        String response = responseBuilder.toString();
        List<SearchResult> parsedResponse = parseResponse(response);
        in.close();
        out.close();
        clientSocket.close();
        return parsedResponse;

    }

    private List<SearchResult> parseResponse(String response) {
        log.info("got response: \n" + response);
        Matcher matcher = responsePattern.matcher(response);
        List<SearchResult> resultList = new LinkedList<>();
        while (matcher.find()) {
            String title = matcher.group("title");
            String url = matcher.group("url");
            resultList.add(new SearchResult(url + " " + title, url));
        }
        log.info("<--- " + resultList.toString());
        return resultList;
    }

}
