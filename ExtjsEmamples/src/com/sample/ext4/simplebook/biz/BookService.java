package com.sample.ext4.simplebook.biz;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;

import com.sample.ext4.simplebook.bean.Book;
import com.sample.ext4.simplebook.bean.BookType;
import com.sample.ext4.simplebook.dao.BookDao;

/**
 * 简易图书管理系统业务类
 */
public class BookService {
	/**
	 * Logger for this class
	 */
	private static final Logger logger = Logger.getLogger(BookService.class);
	
	private BookDao dao = new BookDao();
	/**
	 * 查询书籍类型列表
	 * @return 书籍列席列表
	 */
	public List<BookType> getBookTypes(){
		List<BookType> bookTypes;
		try {
			bookTypes = dao.getBookTypes();
		} catch (SQLException e) {
			bookTypes = new ArrayList<BookType>();
			logger.error(e,e);
		}
		return bookTypes;
	}
	/**
	 * 查询指定书籍类型
	 * @param bookTypeId 书籍类型id
	 * @return 书籍类型对象
	 */
	public BookType getBookType(int bookTypeId){
		BookType bookType = null;
		try {
			bookType = dao.getBookType(bookTypeId);
		} catch (SQLException e) {
			logger.error(e,e);
		}
		return bookType;
	}
	/**
	 * 新增书籍类型
	 * @param bookType 书籍类型对象
	 * @return 类型id
	 */
	public int addBookType(BookType bookType){
		int bookTypeId = -1;
		try {
			bookTypeId = dao.addBookType(bookType);
		} catch (SQLException e) {
			logger.error(e,e);
		}
		return bookTypeId;
	}
	/**
	 * 更新书籍类型
	 * @param bookType 书籍类型对象
	 * @return 增加成功返回true，否则返回false
	 */
	public boolean updateBookType(BookType bookType){
		boolean result = true;
		try {
			dao.updateBookType(bookType);
		} catch (SQLException e) {
			result = false;
			logger.error(e,e);
		}
		return result;
	}
	/**
	 *  查询某种类型书籍的数量
	 * @param bookTypeId 书籍类型id
	 * @return 书籍数量
	 */
	public int getBookCount(int bookTypeId){
		int count = -1;
		try {
			count = dao.getBookNum( bookTypeId);
		} catch (SQLException e) {
			logger.error(e,e);
		}
		return count;
	}
	/**
	 * 删除书籍类型
	 * @param bookTypeId 书籍类型id
	 * @return 删除成功返回true，否则返回false
	 */
	public boolean deleteBookType(int bookTypeId){
		boolean result = true;
		try {
			dao.delBookType(bookTypeId);
		} catch (SQLException e) {
			result = false;
			logger.error(e,e);
		}
		return result;
	}
	/**
	 * 查询书籍列表
	 * @return 书籍列表
	 */
	public List<Book> getBooks(){
		List<Book> books = null;
		try {
			books = dao.getBooks();
		} catch (SQLException e) {
			books = new ArrayList<Book>();
			logger.error(e,e);
		}
		return books;
	}
	/**
	 * 查询指定书籍
	 * @param bookId 书籍id
	 * @return 书籍对象
	 */
	public Book getBook(int bookId){
		Book book = null;
		try {
			book = dao.getBook(bookId);
		} catch (SQLException e) {
			logger.error(e,e);
		}
		return book;
	}
	/**
	 * 增加书籍
	 * @param book 书籍对象
	 * @return 书籍id
	 */
	public int addBook(Book book){
		int bookId = -1;
		try {
			bookId = dao.addBook(book);
		} catch (SQLException e) {
			logger.error(e,e);
		}
		return bookId;
	}
	/**
	 * 更新书籍信息
	 * @param book 书籍对象
	 * @return 更新书籍成功返回true，否则返回false
	 */
	public boolean updateBook(Book book){
		boolean result = true;
		try {
			dao.updateBook(book);
		} catch (SQLException e) {
			result = false;
			logger.error(e,e);
		}
		return result;
	}
	/**
	 * 删除书籍
	 * @param bookId 书籍
	 * @return 删除书籍成功返回true，否则返回false
	 */
	public boolean deleteBook(Book book){
		List<Book> books = new ArrayList<Book>();
		books.add(book);
		return this.deleteBooks(books);
	}
	
	/**
	 * 成批删除书籍信息
	 * @param books 书籍列表
	 * @return 成批删除书籍成功返回true，否则返回false
	 */
	public boolean deleteBooks(List<Book> books){
		boolean result = true;
		try {
			dao.delBooks(books);
		} catch (SQLException e) {
			result = false;
			logger.error(e,e);
		}
		return result;
	}
}