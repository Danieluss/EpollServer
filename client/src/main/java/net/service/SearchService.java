package net.service;

import lombok.extern.slf4j.Slf4j;
import net.data.SearchResult;
import net.data.SearchResultWrapper;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import javax.annotation.Resource;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@Service
@Slf4j
public class SearchService {

    @Resource
    Socket clientSocket;
    private Pattern resultPattern;
    private Pattern responseHeaderPattern;
    private final static String HEADER_SEPARATOR = "-\n";
    private final static String BODY_SEPARATOR = ";\n";
    private final static String RESPONSE_HEADER_PATTERN = "(?<status>#.+?)" + HEADER_SEPARATOR + "(?<totalPages>[0-9]+?)" + HEADER_SEPARATOR;
    private final static String RESULT_PATTERN = "(?<url>.+?)" + BODY_SEPARATOR + "(?<title>.*?)" + BODY_SEPARATOR + "(?<description>.*?)" + BODY_SEPARATOR;

    @PostConstruct
    public void init() throws IOException {
        resultPattern = Pattern.compile(RESULT_PATTERN);
        responseHeaderPattern = Pattern.compile(RESPONSE_HEADER_PATTERN);
    }

    public SearchResultWrapper requestSearch(int page, int pageSize, String searchQuery) throws IOException {
        PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
        BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        searchQuery = page + " " + pageSize + " " + searchQuery;
        log.info("---> " + searchQuery);
        out.println(searchQuery);
        StringBuilder responseBuilder = new StringBuilder();
        String responseLine;
        while ((responseLine = in.readLine()) != null) {
            responseBuilder.append(responseLine);
            responseBuilder.append('\n');
        }
        String response = responseBuilder.toString();
        SearchResultWrapper parsedResponse = parseResponse(response);
        in.close();
        out.close();
        clientSocket.close();
        return parsedResponse;

    }

    private SearchResultWrapper parseResponse(String response) {
        log.info("got response: \n" + response);
        Matcher matcher = responseHeaderPattern.matcher(response);
        SearchResultWrapper searchResultWrapper = new SearchResultWrapper();
        while (matcher.find()) {
            searchResultWrapper.setTotalPages(Integer.parseInt(matcher.group("totalPages")));
        }
        matcher = resultPattern.matcher(response);
        List<SearchResult> resultList = new LinkedList<>();
        while (matcher.find()) {
            String title = matcher.group("title");
            String url = matcher.group("url");
            String description = matcher.group("description");
            resultList.add(new SearchResult(url, title, description));
        }
        searchResultWrapper.setSearchResults(resultList);
        log.info("<--- " + resultList.toString());
        return searchResultWrapper;
    }

}
