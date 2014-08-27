package com.victor.utilities.datastructures;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

import com.victor.utilities.datastructures.Trie.SuffixTrie;

public class SuffixTrieTests {

    @Test
    public void testSuffixTrie() {
        String bookkeeper = "bookkeeper";
        SuffixTrie<String> trie = new SuffixTrie<String>(bookkeeper);
        assertTrue(SuffixTreeTest.suffixTreeTest(trie, bookkeeper));
    }
}
