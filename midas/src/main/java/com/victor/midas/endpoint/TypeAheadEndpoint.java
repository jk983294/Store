package com.victor.midas.endpoint;

import com.victor.midas.dao.StockInfoDao;
import com.victor.midas.endpoint.response.TypeAheadResponse;
import com.victor.midas.services.StocksService;
import com.victor.midas.services.TypeAhead;
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

    @GET
    @RequestMapping("/{query}")
    @Produces(MediaType.APPLICATION_JSON)
    public TypeAheadResponse getTips(@PathVariable("query") String query) {
        logger.info(query);
        List<String> tips = getTipList(query);
        TypeAheadResponse response = new TypeAheadResponse(tips, "SUCCESS");
        return response;
    }


    @GET
    @RequestMapping("/action/{query}")
    @Produces(MediaType.APPLICATION_JSON)
    public TypeAheadResponse getAction(@PathVariable("query") String query) {
        List<String> tips = getTipList(query);
        TypeAheadResponse response = new TypeAheadResponse();
        if(tips.size() > 0){
            response.setAction(tips.get(0));
            response.setStatus("SUCCESS");
        } else {
            response.setStatus("No Action tip");
        }
        return response;
    }

    private List<String> getTipList(String query){
        List<String> totalTips = new ArrayList<>();
        if(query != null){
            String[] stringlets = query.split(" ");
            for (int i = 0; i < stringlets.length; i++) {
                List<String> subtips = typeAhead.query(query);
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
                            totalTips.add(s1 + subtips2.get(j));
                        }
                    }
                }
            }
        }
        return totalTips;
    }
}
