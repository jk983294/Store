package com.victor.finFriday.data_structures.test;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

import com.victor.finFriday.data_structures.SuffixTree;
import com.victor.finFriday.data_structures.test.common.SuffixTreeTest;

public class SuffixTreeTests {

    @Test
    public void testSuffixTree() {
        String bookkeeper = "bookkeeper";
        SuffixTree<String> tree = new SuffixTree<String>(bookkeeper);
        assertTrue(SuffixTreeTest.suffixTreeTest(tree, bookkeeper));
    }
}
