package com.voctor.midas.worker.common;

import com.victor.midas.worker.common.TaskMgr;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

/**
 * unit test for TaskMgr
 */
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:/WEB-INF/midas-content.xml")
public class TaskMgrTest {

    @Autowired
    private TaskMgr taskMgr;


    @Test
    public void testTaskCmd(){
        taskMgr.cmd("bad cmd");
    }
}
