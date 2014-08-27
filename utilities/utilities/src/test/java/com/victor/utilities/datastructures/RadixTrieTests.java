package com.victor.utilities.datastructures;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

import com.victor.utilities.datastructures.Trie.RadixTrie;
import com.victor.utilities.datastructures.Utils.*;

public class RadixTrieTests {

    @Test
    public void testRadixTrie() {
        TestData data = Utils.generateTestData(1000);

        String mapName = "RadixTrie";
        RadixTrie<String,Integer> map = new RadixTrie<String,Integer>();
        java.util.Map<String,Integer> jMap = map.toMap();

        assertTrue(MapTest.testMap(map, Type.String, mapName,
                                   data.unsorted, data.invalid));
        assertTrue(JavaMapTest.testJavaMap(jMap, Type.String, mapName,
                                           data.unsorted, data.sorted, data.invalid));
    }

}
