Value http_create_server(const std::vector<Value>& n);
Value http_server_get(const std::vector<Value>& n);
Value http_server_stop(const std::vector<Value>& n);
Value http_server_listen(const std::vector<Value>& n);
Value http_server_bind_to_any_port(const std::vector<Value>& n);
Value http_server_listen_after_bind(const std::vector<Value>& n);
Value http_server_set_mount_point(const std::vector<Value>& n);
Value http_server_remove_mount_point(const std::vector<Value>& n);
Value http_server_set_fext_mimetype(const std::vector<Value>& n);
Value http_server_enable_logger(const std::vector<Value>& n);

Value http_create_headers(const std::vector<Value>& n);
Value http_create_client(const std::vector<Value>& n);
Value http_client_get(const std::vector<Value>& n);