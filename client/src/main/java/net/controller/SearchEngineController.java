package net.controller;

import net.data.SearchResult;
import net.service.SearchService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;

@Controller
public class SearchEngineController {

    @Autowired
    private SearchService searchService;

    @RequestMapping(value = "/", method = {RequestMethod.GET, RequestMethod.POST})
    public String search(@RequestParam(required = false) String searchQuery, Model model) throws IOException {
        if (searchQuery != null) {
            List<SearchResult> searchResults = searchService.requestSearch(searchQuery);
        }
        return "index";
    }

}
