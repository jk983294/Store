package com.victor.midas.endpoint;

import com.victor.midas.dao.StockInfoDao;
import com.victor.midas.endpoint.response.TypeAheadResponse;
import com.victor.midas.services.StocksService;
import com.victor.midas.services.TaskMgr;
import com.victor.midas.services.TypeAhead;
import com.victor.midas.util.MidasConstants;
import com.victor.midas.util.StringPatternAware;
import org.apache.commons.lang3.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.ws.rs.GET;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import java.util.ArrayList;
import java.util.List;

/**
 * for type ahead
 */
@RestController
@RequestMapping("/typeahead")
public class TypeAheadEndpoint {
    private final Logger logger = Logger.getLogger(TypeAheadEndpoint.class);

    @Autowired
    private StockInfoDao stockInfoDao;
    @Autowired
    private TypeAhead typeAhead;
    @Autowired
    private StocksService stocksService;
    @Autowired
    private TaskMgr taskMgr;

    private static final int MAX_TIPS_ENTRY = 10;

    @GET
    @RequestMapping("/{query}")
    @Produces(MediaType.APPLICATION_JSON)
    public TypeAheadResponse getTips(@PathVariable("query") String query) {
        List<String> tips = getTipList(query);
        TypeAheadResponse response = new TypeAheadResponse(tips, MidasConstants.RESPONSE_SUCCESS, null);
        return response;
    }


    /**
     * process query, always get first auto-complete string for sub-query,
     * if could not find matched auto-complete string, use original string
     */
    @GET
    @RequestMapping("/action/{query}")
    @Produces(MediaType.APPLICATION_JSON)
    public TypeAheadResponse getAction(@PathVariable("query") String query) {
        TypeAheadResponse response = new TypeAheadResponse();
        StringBuilder tip = new StringBuilder("");
        try {
            if(query != null){
                String[] stringlets = query.split(" ");
                for (int i = 0; i < stringlets.length; i++) {
                    List<String> subtips = typeAhead.query(stringlets[i]);
                    if(subtips.size() > 0) {
                        tip.append(subtips.get(0)).append(" ");
                    } else {
                        tip.append(stringlets[i]).append(" ");
                    }
                }

                String tipStr = tip.toString().trim();
                //logger.info(query + " " + tipStr);
                response.setAction(tipStr);
                dealWithAction(tipStr, response);

            } else {
                response.setStatus(MidasConstants.RESPONSE_FAIL);
                response.setDescription("No query string");
            }
        } catch (Exception e){
            response.setStatus(MidasConstants.RESPONSE_FAIL);
            response.setDescription("error caused by : " + e);
        }
        return response;
    }

    /**
     * deal with action string
     */
    private void dealWithAction(String action, TypeAheadResponse response ){
        String[] actions = action.split(" ");
        StringBuilder responseStr = new StringBuilder();
        switch (actions.length){
            case 1 : {
                switch (actions[0].toLowerCase()){
                    case "load" : {
                        taskMgr.cmd(TaskMgr.CMD_LOAD_DATA);
                        responseStr.append("load task is submitted.");
                    }break;
                    case "delete" : {
                        taskMgr.cmd(TaskMgr.CMD_DELETE_DATE);
                        responseStr.append("delete task is submitted.");
                    }break;
                    case "create" : {
                        taskMgr.cmd(TaskMgr.CMD_CREATE_DB);
                        responseStr.append("delete task is submitted.");
                    }break;
                    default: {
                        if(StringPatternAware.isStockCode(actions[0])){
                            responseStr.append("jump to stock detail.");
                        } else {
                            responseStr.append("no matched cmd is found");
                            response.setStatus(MidasConstants.RESPONSE_FAIL);
                        }
                    }

                    break;    // deal with 1 sub tip ends
                }
            } break;    // action array has 1 sub tip ends
            default: {
                responseStr.append("no action recognized");
            } break;
        }
        response.setDescription(responseStr.toString());
        if(response.getStatus() == null ){
            response.setStatus(MidasConstants.RESPONSE_SUCCESS);
        }
    }

    /**
     * split query string, get each sub query string's tips, combine all sub query strings' tips
     */
    private List<String> getTipList(String query){
        List<String> totalTips = new ArrayList<>();
        if(query != null){
            String[] stringlets = query.split(" ");
            int currentMaxtipEntryCnt = MAX_TIPS_ENTRY;
            for (int i = 0; i < stringlets.length; i++) {
                List<String> subtips = typeAhead.query(stringlets[i]);
                // if current subtips is more than currentMaxtipEntryCnt, cut it to current max
                if( currentMaxtipEntryCnt < subtips.size()){
                    subtips = subtips.subList(0, currentMaxtipEntryCnt);
                }
                if(subtips.size() > 0) {
                    currentMaxtipEntryCnt = Math.max( 1, (int) Math.ceil(currentMaxtipEntryCnt / subtips.size()));
                }

                totalTips = mergeTips(totalTips, subtips);
            }
        }
        return totalTips;
    }

    /**
     * get subtips1 * subtips2
     */
    private List<String> mergeTips(List<String> subtips1, List<String> subtips2){
        List<String> totalTips = new ArrayList<>();
        if(subtips1.size() == 0){
            totalTips.addAll(subtips2);
        } else if(subtips2.size() == 0){
            totalTips.addAll(subtips1);
        } else {
            for (int i = 0; i < subtips1.size(); i++) {
                if(StringUtils.isNotEmpty(subtips1.get(i))){
                    String s1 = subtips1.get(i);
                    for (int j = 0; j < subtips2.size(); j++) {
                        if(StringUtils.isNotEmpty(subtips2.get(j))){
                            totalTips.add(s1 + " " + subtips2.get(j));
                        }
                    }
                }
            }
        }
        return totalTips;
    }
}
