package com.victor.finFriday.data_structures.test;

import static org.junit.Assert.assertTrue;

import java.util.Collection;

import org.junit.Test;

import com.victor.finFriday.data_structures.BinarySearchTree;
import com.victor.finFriday.data_structures.SplayTree;
import com.victor.finFriday.data_structures.test.common.JavaCollectionTest;
import com.victor.finFriday.data_structures.test.common.TreeTest;
import com.victor.finFriday.data_structures.test.common.Utils;
import com.victor.finFriday.data_structures.test.common.Utils.TestData;
import com.victor.finFriday.data_structures.test.common.Utils.Type;

public class SplayTreeTests {

    @Test
    public void testSplayTree() {
        TestData data = Utils.generateTestData(1000);

        String bstName = "Splay Tree";
        BinarySearchTree<Integer> bst = new SplayTree<Integer>();
        Collection<Integer> bstCollection = bst.toCollection();

        assertTrue(TreeTest.testTree(bst, Type.Integer, bstName,
                                     data.unsorted, data.invalid));
        assertTrue(JavaCollectionTest.testCollection(bstCollection, Type.Integer, bstName,
                                                     data.unsorted, data.sorted, data.invalid));
    }

}
