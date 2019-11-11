package net.service;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Scope;

import java.io.IOException;
import java.net.Socket;

@Configuration
public class SearchServiceConfig {

    @Value("${searchServer.ip}")
    private String searchServerIP;

    @Value("${searchServer.port}")
    private Integer searchServerPort;

    @Bean
    @Scope("prototype")
    public Socket clientSocket() throws IOException {
        return new Socket(searchServerIP, searchServerPort);
    }

}
