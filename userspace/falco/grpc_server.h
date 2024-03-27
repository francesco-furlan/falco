// SPDX-License-Identifier: Apache-2.0
/*
Copyright (C) 2023 The Falco Authors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include <thread>
#include <string>
#include <atomic>

#include "outputs.grpc.pb.h"
#include "version.grpc.pb.h"
#include "grpc_context.h"

namespace falco
{
namespace grpc
{

class server
{
public:
	server() = default;
	virtual ~server() = default;

	void init(
		const std::string& server_addr,
		int threadiness,
		const std::string& private_key,
		const std::string& cert_chain,
		const std::string& root_certs,
		const std::string& log_level
	);
	void thread_process(int thread_index);
	void run();
	void stop();
	void shutdown();

	outputs::service::AsyncService m_output_svc;
	version::service::AsyncService m_version_svc;

	std::unique_ptr<::grpc::ServerCompletionQueue> m_completion_queue;

private:
	std::string m_server_addr;
	int m_threadiness = 1;
	std::string m_private_key;
	std::string m_cert_chain;
	std::string m_root_certs;

	std::vector<std::thread> m_threads;
	::grpc::ServerBuilder m_server_builder;
	void init_mtls_server_builder();
	void init_unix_server_builder();

	bool is_running();

	// Outputs
	void get(const stream_context& ctx, const outputs::request& req, outputs::response& res);
	void sub(const bidi_context& ctx, const outputs::request& req, outputs::response& res);

	// Version
	void version(const context& ctx, const version::request& req, version::response& res);

	std::unique_ptr<::grpc::Server> m_server;

	std::atomic<bool> m_stop{false};
};

} // namespace grpc
} // namespace falco
