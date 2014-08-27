package com.victor.utilities.datastructures;

import static org.junit.Assert.assertTrue;

import java.util.Collection;

import org.junit.Test;

import com.victor.utilities.datastructures.Utils.*;
import com.victor.utilities.datastructures.tree.BinarySearchTree;
import com.victor.utilities.datastructures.tree.SplayTree;

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
