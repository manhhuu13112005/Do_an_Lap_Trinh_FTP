# FTPv3 Client-Server File Transfer System

## Giới thiệu

Đây là đồ án môn **Lập trình mạng hệ thống**, xây dựng chương trình truyền file theo mô hình **Client – Server** dựa trên giao thức FTP đơn giản hóa.

Hệ thống cho phép Client kết nối tới Server, đăng nhập bằng `IP`, `username`, `password`, sau đó upload một hoặc nhiều file trong thư mục mặc định lên Server. Server sẽ xác thực người dùng, nhận file và lưu vào thư mục riêng theo từng username.

## Chức năng chính

### Server
- Khởi động và dừng dịch vụ FTP Server.
- Lắng nghe kết nối từ Client qua TCP Socket.
- Xác thực tài khoản người dùng.
- Nhận file từ Client thông qua lệnh `SUBMIT`.
- Lưu file vào thư mục riêng theo username.
- Hỗ trợ nhiều Client kết nối đồng thời bằng cơ chế đa luồng.
- Kiểm tra tên file để hạn chế lỗi Path Traversal.

### Client
- Kết nối tới Server bằng địa chỉ IP và port.
- Đăng nhập bằng username và password.
- Quét thư mục mặc định `upload_client`.
- Upload một file hoặc toàn bộ file trong thư mục lên Server.
- Hiển thị trạng thái upload và phản hồi từ Server.
- Gửi lệnh `QUIT` để đóng kết nối an toàn.

## Giao thức sử dụng

Hệ thống sử dụng giao tiếp TCP Socket với các lệnh dạng văn bản:

```text
220 OK      - Kết nối thành công
230 OK      - Đăng nhập thành công
530 FAIL    - Đăng nhập thất bại
150 OK      - Server sẵn sàng nhận file
226 OK      - Nhận file thành công
426 ERROR   - Lỗi trong quá trình truyền file
221 BYE     - Đóng kết nối
login <ip> <username> <password>
SUBMIT <filename> <size>
QUIT
