package net.controller;

import net.data.SearchResult;
import net.service.SearchService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import java.io.IOException;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

@Controller
public class SearchEngineController {

    private final SearchService searchService;

    public SearchEngineController(SearchService searchService) {
        this.searchService = searchService;
    }

    @RequestMapping(value = "/", method = {RequestMethod.GET, RequestMethod.POST})
    public String search(@RequestParam(required = false) String searchQuery, Model model) throws IOException {
        model.addAttribute("searchQuery", searchQuery);
        if (searchQuery != null && !searchQuery.isEmpty()) {
            model.addAttribute("searchResults", searchService.requestSearch(searchQuery));
        } else {
            model.addAttribute("searchError", "Search query is empty!");
        }
        return "index";
    }

}
