package com.victor.midas.util;

/**
 * exception for index calculation
 */
public class MidasException extends Exception {

    public MidasException() {
    }

    public MidasException(String message) {
        super(message);
    }

    public MidasException(String message, Throwable cause) {
        super(message, cause);
    }

    public MidasException(Throwable cause) {
        super(cause);
    }

    public MidasException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
