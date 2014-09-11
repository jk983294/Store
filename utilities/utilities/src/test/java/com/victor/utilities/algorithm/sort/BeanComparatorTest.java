package com.victor.utilities.algorithm.sort;

import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;

// A simple "Bean" class for testing only. 
// Does NOT implement comparable!
class Word {
  private final String word;
  public Word(String word) { this.word=word; }
  public String getWord() {  return this.word; }
  public void noop() { }
  public String toString() { return this.word;  }
  public Word notComparable() { return new Word("notComparable"); }
}

class BeanComparatorTest
{
  @SuppressWarnings("unchecked")
  public static void main(String[] args) {
    try { 
      List<Word> words = readWords("WordFinder.txt");

      System.err.println("---------------------------------------------------");

      // SUCCESS CASES

                       //short for new UniversalReflectiveComparator<Word>(Word.class,"toString",ASCENDING);
      try {
        Collections.sort(words, new BeanComparator<Word>(Word.class));
        System.out.println(words);
      } catch (Exception e) {e.printStackTrace();}
      System.err.println("---------------------------------------------------");

      try {
        Collections.sort(words, new BeanComparator<Word>(Word.class, "getWord", BeanComparator.DESCENDING));
      } catch (Exception e) {e.printStackTrace();}
      System.out.println(words);
      System.err.println("---------------------------------------------------");

      try {
        Collections.sort(words, new BeanComparator<Word>(Word.class, BeanComparator.DESCENDING));
      } catch (Exception e) {e.printStackTrace();}
      System.out.println(words);
      System.err.println("---------------------------------------------------");

      // FAIL CASES

      try {
        Collections.sort(words, new BeanComparator<Word>(Word.class, "nonExistantMethodName"));
      } catch (Exception e) {e.printStackTrace();}
      System.err.println("---------------------------------------------------");

      try {
        Collections.sort(words, new BeanComparator<Word>(Word.class, "noop"));
      } catch (Exception e) {e.printStackTrace();}
      System.err.println("---------------------------------------------------");

      try {
        Collections.sort(words, new BeanComparator<Word>(Word.class, "notComparable"));
      } catch (Exception e) {e.printStackTrace();}
      System.err.println("---------------------------------------------------");

    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  /**
   * reads each line of the given file into a List of strings.
   * @param String filename - the name of the file to read
   * @return an List handle ArrayList of strings containing file contents.
   */
  public static List<Word> readWords(String filename) throws FileNotFoundException, IOException {
    List<Word> results = new ArrayList<Word>();
    BufferedReader reader = null;
    try {
      reader = new BufferedReader(new FileReader(filename));
      String line = null;
      while ( (line=reader.readLine()) != null ) {
        results.add(new Word(line));
      }
    } finally {
      if(reader!=null)reader.close();
    }
    return results;
  }

}
