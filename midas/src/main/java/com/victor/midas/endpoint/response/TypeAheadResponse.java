package com.victor.midas.endpoint.response;

import java.util.List;

/**
 * Type Ahead Response
 */
public class TypeAheadResponse {

    private String action;
    private List<String> tips;
    private String status;

    public TypeAheadResponse() { }

    public TypeAheadResponse(List<String> tips, String status) {
        this.tips = tips;
        this.status = status;
    }

    public TypeAheadResponse(String action, String status) {
        this.action = action;
        this.status = status;
    }

    public String getAction() {
        return action;
    }

    public void setAction(String action) {
        this.action = action;
    }

    public List<String> getTips() {
        return tips;
    }

    public void setTips(List<String> tips) {
        this.tips = tips;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    @Override
    public String toString() {
        return "TypeAheadResponse{" +
                "action='" + action + '\'' +
                ", tips=" + tips +
                ", status='" + status + '\'' +
                '}';
    }
}
