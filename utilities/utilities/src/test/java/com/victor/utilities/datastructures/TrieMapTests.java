package com.victor.utilities.datastructures;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

import com.victor.utilities.datastructures.Trie.TrieMap;
import com.victor.utilities.datastructures.Utils.*;

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
