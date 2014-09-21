package com.victor.midas.dao;

import org.apache.log4j.Logger;
import org.springframework.stereotype.Component;

/**
 * Dao for index collection
 */
@Component
public class IndexDao {
    private final String COLLECTION_NAME = "stockIndex";

    private static final Logger logger = Logger.getLogger(IndexDao.class);

}
