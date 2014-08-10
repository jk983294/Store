package com.victor.finFriday.data_structures.test;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

import com.victor.finFriday.data_structures.TrieMap;
import com.victor.finFriday.data_structures.test.common.JavaMapTest;
import com.victor.finFriday.data_structures.test.common.MapTest;
import com.victor.finFriday.data_structures.test.common.Utils;
import com.victor.finFriday.data_structures.test.common.Utils.TestData;
import com.victor.finFriday.data_structures.test.common.Utils.Type;

public class TrieMapTests {

    @Test
    public void testTrieMap() {
        TestData data = Utils.generateTestData(1000);

        String mapName = "TrieMap";
        TrieMap<String,Integer> map = new TrieMap<String,Integer>();
        java.util.Map<String,Integer> jMap = map.toMap();

        assertTrue(MapTest.testMap(map, Type.String, mapName,
                                   data.unsorted, data.invalid));
        assertTrue(JavaMapTest.testJavaMap(jMap, Type.String, mapName,
                                           data.unsorted, data.sorted, data.invalid));
    }
}
