/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   openssl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/18 02:19:49 by amineau           #+#    #+#             */
/*   Updated: 2022/04/22 21:44:03 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void ShowCerts(SSL *ssl)
{
	X509 *cert;
	char *line;

	cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
	if (cert != NULL)
	{
		printf("Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("Subject: %s\n", line);
		free(line); /* free the malloc'ed string */
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("Issuer: %s\n", line);
		free(line);      /* free the malloc'ed string */
		X509_free(cert); /* free the malloc'ed certificate copy */
	}
	else
		printf("Info: No client certificates configured.\n");
}

void init_openssl()
{
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl()
{
	ERR_free_strings();
	EVP_cleanup();
}

void shutdown_ssl(SSL *ssl)
{
	SSL_shutdown(ssl);
	SSL_free(ssl);
}

SSL_CTX *ftp_srv_create_context()
{
	const SSL_METHOD *method;
	SSL_CTX          *ctx;

	method = TLS_server_method();

	ctx = SSL_CTX_new(method);
	if (!ctx)
		error_print_exit(EXIT_FAILURE, "Unable to create SSL context");

	return ctx;
}

SSL_CTX *ftp_cli_create_context()
{
	const SSL_METHOD *method;
	SSL_CTX          *ctx;

	method = TLS_client_method();

	ctx = SSL_CTX_new(method);
	if (!ctx)
		error_print_exit(EXIT_FAILURE, "Unable to create SSL context");

	return ctx;
}

void configure_context(SSL_CTX *ctx)
{
	SSL_CTX_set_ecdh_auto(ctx, 1);

	if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0)
		error_print_exit(EXIT_FAILURE, "Error on cert use");

	if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0)
		error_print_exit(EXIT_FAILURE, "Error on key use");
}

SSL *ftp_create_ssl(int sock, SSL_CTX *ctx)
{
	SSL *ssl;

	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, sock);
	return ssl;
}

void ftp_connect_ssl(SSL *ssl)
{
	if (SSL_connect(ssl) <= 0)
		error_print_exit(EXIT_FAILURE, "SSL_connect failed");
}