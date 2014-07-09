package com.sample.ext4.simplebook.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import com.sample.ext4.simplebook.bean.Book;
import com.sample.ext4.simplebook.bean.BookType;
import com.sample.ext4.simplebook.util.MyDBUtil;
/**
 * 简单图书管理系统数据访问类
 */
public class BookDao {
	
	/**
	 * 查询某种类型书籍的数量
	 * @param con
	 * @param bookTypeId 书籍类型id
	 * @return 书籍数量
	 * @throws SQLException
	 * @throws ClassNotFoundException 
	 */
	public int getBookNum(int bookTypeId) throws SQLException{
		Connection conn = MyDBUtil.getConnection();
		String sql = "select count(*) from book where type_id = ?";
		PreparedStatement pst = conn.prepareStatement(sql);
		pst.setInt(1, bookTypeId);
		ResultSet rs = pst.executeQuery();
		int num = 0 ;
		while(rs.next()){
			num = rs.getInt(1);
		}
		rs.close();
		pst.close();
		conn.close();
		return num;
	}
	
	/**
	 * 查询全部书籍类型列表
	 * @param con
	 * @return 书籍类型列表
	 * @throws SQLException
	 * @throws ClassNotFoundException
	 */
	public List<BookType> getBookTypes() throws SQLException{
		Connection conn = MyDBUtil.getConnection();
		List<BookType> bookTypes = new ArrayList<BookType>();
		Statement st = conn.createStatement();
		String sql = "select id,title,detail from booktype";
		ResultSet rs = st.executeQuery(sql);
		while(rs.next()){
			BookType bookType = new BookType();
			bookType.setId(rs.getInt("id"));
			bookType.setTitle(rs.getString("title"));
			bookType.setDetail(rs.getString("detail"));
			bookTypes.add(bookType);
		}
		rs.close();
		st.close();
		conn.close();
		return bookTypes;
	}
	
	/**
	 * 查询指定id的书籍类垄1�7
	 * @param con
	 * @param bookTypeId 书籍类型id
	 * @return 书籍类型对象
	 * @throws SQLException
	 * @throws ClassNotFoundException
	 */
	public BookType getBookType(int bookTypeId) throws SQLException{
		Connection conn = MyDBUtil.getConnection();
		String sql = "select id,title,detail from booktype where id = ?";
		PreparedStatement pst = conn.prepareStatement(sql);
		pst.setInt(1, bookTypeId);
		ResultSet rs = pst.executeQuery();
		BookType bookType = null;
		while(rs.next())
		{
			bookType = new BookType();
			bookType.setId(rs.getInt("id"));
			bookType.setTitle(rs.getString("title"));
			bookType.setDetail(rs.getString("detail"));
		}
		rs.close();
		pst.close();
		conn.close();
		return bookType;
	}
	
	/**
	 * 新增书籍类型
	 * @param bookType 书籍类型对象
	 * @return 书籍类型id
	 * @throws SQLException
	 */
	public int addBookType(BookType bookType) throws SQLException{
		Connection con = MyDBUtil.getConnection();
		String sql = "insert into booktype(title,detail) values(?,?)";
		PreparedStatement pst = con.prepareStatement(sql,Statement.RETURN_GENERATED_KEYS);
		pst.setString(1, bookType.getTitle());
		pst.setString(2, bookType.getDetail());
		pst.execute();
		ResultSet rs = pst.getGeneratedKeys();
		rs.next();
		int id = rs.getInt("id");
		rs.close();
		pst.close();
		con.close();
		return id;
	}
	
	/**
	 * 修改书籍类型
	 * @param con
	 * @param bookType 书籍类型对象
	 * @throws SQLException
	 */
	public void updateBookType(BookType bookType) throws SQLException{
		Connection conn = MyDBUtil.getConnection();
		String sql = "update booktype set title = ?,detail = ? where id = ?";
		PreparedStatement pst = conn.prepareStatement(sql);
		pst.setString(1, bookType.getTitle());
		pst.setString(2, bookType.getDetail());
		pst.setInt(3, bookType.getId());
		pst.execute();
		pst.close();
		conn.close();
	}
	
	/**
	 * 删除书籍类型
	 * @param con
	 * @param bookType 书籍类型id
	 * @throws SQLException
	 */
	public void delBookType(int bookTypeId) throws SQLException{
		Connection conn = MyDBUtil.getConnection();
		String sql = "delete from booktype where id = ?";
		PreparedStatement pst = conn.prepareStatement(sql);
		pst.setInt(1, bookTypeId);
		pst.execute();
		pst.close();
		conn.close();
	}
	
	/**
	 * 查询书籍列表
	 * @param con
	 * @return 书籍列表
	 * @throws SQLException
	 * @throws ClassNotFoundException
	 */
	public List<Book> getBooks() throws SQLException{
		List<Book> books = new ArrayList<Book>();
		Connection conn = MyDBUtil.getConnection();
		Statement st = conn.createStatement();
		String sql = "select b.id,b.book_name,b.author,b.type_id,t.title,b.price,b.brief from book b,booktype t where b.type_id = t.id order by b.id";
		ResultSet rs = st.executeQuery(sql);
		while(rs.next()){
			Book book = new Book();
			book.setId(rs.getInt("id"));
			book.setBookName(rs.getString("book_name"));
			book.setAuthor(rs.getString("author"));
			book.setBookTypeId(rs.getInt("type_id"));
			book.setTypeName(rs.getString("title"));
			book.setPrice(rs.getFloat("price"));
			book.setBrief(rs.getString("brief"));
			books.add(book);
		}
		rs.close();
		st.close();
		conn.close();
		return books;
	}
	
	/**
	 * 查询指定id的书籍信恄1�7
	 * @param con
	 * @param bookId 书籍id
	 * @return 书籍对象
	 * @throws SQLException
	 * @throws ClassNotFoundException
	 */
	public Book getBook(int bookId) throws SQLException{
		Connection conn = MyDBUtil.getConnection();
		String sql = "select b.id,b.book_name,b.author,b.type_id,t.title,b.price,b.brief from book b,booktype t where b.type_id = t.id and b.id=?";
		PreparedStatement pst = conn.prepareStatement(sql);
		pst.setInt(1, bookId);
		ResultSet rs = pst.executeQuery();
		Book book = null;
		if(rs.next()){
			book = new Book();
			book.setId(rs.getInt("id"));
			book.setBookName(rs.getString("book_name"));
			book.setAuthor(rs.getString("author"));
			book.setBookTypeId(rs.getInt("type_id"));
			book.setTypeName(rs.getString("title"));
			book.setPrice(rs.getFloat("price"));
			book.setBrief(rs.getString("brief"));
		}
		rs.close();
		pst.close();
		conn.close();
		return book;
	}
	
	/**
	 * 新增书籍
	 * @param book 书籍对象
	 * @return 书籍id
	 * @throws SQLException
	 */
	public int addBook(Book book) throws SQLException{
		Connection conn = MyDBUtil.getConnection();
		String sql = "insert into book(book_name,author,type_id,price,brief) values(?,?,?,?,?)";
		PreparedStatement pst = conn.prepareStatement(sql,Statement.RETURN_GENERATED_KEYS);
		pst.setString(1, book.getBookName());
		pst.setString(2, book.getAuthor());
		pst.setInt(3, book.getBookTypeId());
		pst.setFloat(4, book.getPrice());
		pst.setString(5, book.getBrief());
		pst.execute();
		ResultSet rs = pst.getGeneratedKeys();
		rs.next();
		int id = rs.getInt("id");
		rs.close();
		pst.close();
		conn.close();
		return id;
	}
	
	/**
	 * 修改书籍信息
	 * @param con
	 * @param book 书籍对象
	 * @throws SQLException
	 */
	public void updateBook(Book book) throws SQLException{
		Connection conn = MyDBUtil.getConnection();
		String sql = "update book set book_name = ?,author = ? ,type_id = ? ,price = ? ,brief = ? where id = ?";
		PreparedStatement pst = conn.prepareStatement(sql);
		pst.setString(1, book.getBookName());
		pst.setString(2, book.getAuthor());
		pst.setInt(3, book.getBookTypeId());
		pst.setFloat(4, book.getPrice());
		pst.setString(5, book.getBrief());
		pst.setInt(6, book.getId());
		pst.execute();
		pst.close();
		conn.close();
	}
	
	/**
	 * 删除书籍
	 * @param con
	 * @param bookId 书籍id
	 * @throws SQLException
	 */
	public void delBooks(List<Book> books) throws SQLException{
		Connection conn = MyDBUtil.getConnection();
		String sql = "delete from book where id = ?";
		PreparedStatement pst = conn.prepareStatement(sql);
		for(int i = 0; i < books.size(); i++){
			Book book = books.get(i);
			pst.setInt(1, book.getId());
			pst.addBatch();
		}
		pst.executeBatch();
		pst.close();
		conn.close();
	}
}