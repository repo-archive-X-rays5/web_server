SetPort(8000)
SetDocumentRoot(os.getenv("source").."\\repos\\web_server\\testdocumentroot")
SetMaxThreads(100)
Https(true)
Http(true)
CertificateFile(os.getenv("source").."\\repos\\web_server\\ssl-certificate\\cert.pem")
PrivateKeyFile(os.getenv("source").."\\repos\\web_server\\ssl-certificate\\privkey.pem")