package com.victor.midas.model.db.misc;

import org.springframework.data.annotation.Id;

/**
 * Misc base class
 */
public abstract class MiscBase {
    @Id
    protected String MiscName;

    protected MiscBase(String miscName) {
        MiscName = miscName;
    }

    public String getMiscName() {
        return MiscName;
    }

    public void setMiscName(String miscName) {
        MiscName = miscName;
    }

    @Override
    public String toString() {
        return "MiscBase{" +
                "MiscName='" + MiscName + '\'' +
                '}';
    }
}
