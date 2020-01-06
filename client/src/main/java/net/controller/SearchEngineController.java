package net.controller;

import net.data.SearchResultWrapper;
import net.service.SearchService;
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

    private static final Integer DEFAULT_PAGE_SIZE = 10;
    private final SearchService searchService;

    public SearchEngineController(SearchService searchService) {
        this.searchService = searchService;
    }

    @RequestMapping(value = "/", method = {RequestMethod.GET, RequestMethod.POST})
    public String search(@RequestParam(required = false) String searchQuery, @RequestParam(required = false) Integer currentPage, Model model) throws IOException {
        model.addAttribute("searchQuery", searchQuery);
        if (currentPage == null) {
            currentPage = 0;
        }
        if (searchQuery != null && !searchQuery.isEmpty()) {
            SearchResultWrapper searchResultWrapper = searchService.requestSearch(currentPage, DEFAULT_PAGE_SIZE, searchQuery);
            List<Integer> pages = IntStream.range(0, Math.min(searchResultWrapper.getTotalPages(), 10)).boxed().collect(Collectors.toList());
            model.addAttribute("searchResults", searchResultWrapper.getSearchResults());
            model.addAttribute("totalPages", searchResultWrapper.getTotalPages());
            model.addAttribute("currentPage", currentPage);
            model.addAttribute("pages", pages);
        } else {
            model.addAttribute("searchError", "Search query is empty!");
        }
        return "index";
    }

}
