package com.victor.utilities.datastructures;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

import com.victor.utilities.datastructures.HashMap;
import com.victor.utilities.datastructures.Utils.*;

public class HashMapTests {

    @Test
    public void testHashMap() {
        TestData data = Utils.generateTestData(1000);

        String mapName = "HashMap";
        HashMap<Integer,String> map = new HashMap<Integer,String>();
        java.util.Map<Integer,String> jMap = map.toMap();

        assertTrue(MapTest.testMap(map, Type.Integer, mapName,
                                   data.unsorted, data.invalid));
        assertTrue(JavaMapTest.testJavaMap(jMap, Type.Integer, mapName,
                                           data.unsorted, data.sorted, data.invalid));
    }
}
