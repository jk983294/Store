package com.voctor.midas.worker.common;

import com.victor.midas.services.TaskMgr;

import org.apache.log4j.Logger;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import java.util.concurrent.TimeUnit;

/**
 * unit test for TaskMgr
 */
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:/WEB-INF/midas-content.xml")
public class TaskMgrTest {

    private static final Logger logger = Logger.getLogger(TaskMgrTest.class);

    @Autowired
    private TaskMgr taskMgr;

    @Test
    public void testLoadMarketDataTaskCmd() throws InterruptedException {
        logger.info("taskMgr.cmd(loadMarketData)");
        taskMgr.cmd("loadMarketData");
        TimeUnit.DAYS.sleep(1);
    }

    @Test
    public void testDeleteAllDataTaskCmd() throws InterruptedException {
        logger.info("taskMgr.cmd(deleteAllData)");
        taskMgr.cmd("deleteAllData");
        TimeUnit.DAYS.sleep(1);
    }

    @Test
    public void testCreateDBTaskCmd() throws InterruptedException {
        logger.info("taskMgr.cmd(createDB)");
        taskMgr.cmd("createDB");
        TimeUnit.DAYS.sleep(1);
    }
}
