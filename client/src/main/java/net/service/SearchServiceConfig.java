package net.service;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Scope;
import org.springframework.context.annotation.ScopedProxyMode;
import org.springframework.web.context.WebApplicationContext;

import java.io.IOException;
import java.net.Socket;

@Configuration
public class SearchServiceConfig {

    @Value("${searchServer.ip}")
    private String searchServerIP;

    @Value("${searchServer.port}")
    private Integer searchServerPort;

    @Bean
    @Scope(value = WebApplicationContext.SCOPE_REQUEST, proxyMode = ScopedProxyMode.TARGET_CLASS)
    public Socket clientSocket() throws IOException {
        return new Socket(searchServerIP, searchServerPort);
    }

}
