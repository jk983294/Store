package com.victor.finFriday.data_structures.test;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

import com.victor.finFriday.data_structures.HashArrayMappedTrie;
import com.victor.finFriday.data_structures.test.common.JavaMapTest;
import com.victor.finFriday.data_structures.test.common.MapTest;
import com.victor.finFriday.data_structures.test.common.Utils;
import com.victor.finFriday.data_structures.test.common.Utils.TestData;
import com.victor.finFriday.data_structures.test.common.Utils.Type;

public class HashArrayMappedTreeTests {

    @Test
    public void testHAMT() {
        TestData data = Utils.generateTestData(1000);

        String mapName = "HAMT";
        HashArrayMappedTrie<Integer,String> map = new HashArrayMappedTrie<Integer,String>();
        java.util.Map<Integer,String> jMap = map.toMap();

        assertTrue(MapTest.testMap(map, Type.Integer, mapName,
                                   data.unsorted, data.invalid));
        assertTrue(JavaMapTest.testJavaMap(jMap, Type.Integer, mapName,
                                           data.unsorted, data.sorted, data.invalid));
    }
}
