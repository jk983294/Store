package com.victor.midas;


import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.ImportResource;

/**
 * used to launch restful services
 */
@ComponentScan
@EnableAutoConfiguration
@ImportResource("classpath:WEB-INF/midas-content.xml")
public class SpringMain {

    public static void main(String[] args) {
        SpringApplication.run(SpringMain.class, args);
    }
}