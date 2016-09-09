#pragma once

// ClientSocket ÃüÁîÄ¿±ê

class ClientSocket : public CAsyncSocket
{
public:
	ClientSocket();
	virtual ~ClientSocket();
	virtual void OnReceive(int nErrorCode);
};


