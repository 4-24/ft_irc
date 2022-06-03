# include <sys/types.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
#include <arpa/inet.h>
​
# include <list>
# include <string>
# include <iostream>
# include <exception>
# include <cstring>
# include <cerrno>
​
#define MYPORT	"9034" // 유저들이 서버에 연결할 포트
#define BACKLOG	10 // 대기 큐에 있을 pending connection 수
#define STDIN	0
​

int	mini_server(char *ip_addr)
{
	int							status;	// 서버의 addrinfo를 받아올 status 값
	int							sock_fd, new_fd; // server socket의 fd, 새로 들어올 client의 fd
	struct addrinfo			hints, *servinfo; // ip 메타데이터를 담은 hints, 소켓 정보를 링크드 리스트로 담을 servinfo
	struct sockaddr_storage	client_addr; // ip 버젼을 모르기때문에 storage로 client는 저장
	socklen_t					addr_size; // client의 addr 구조체 크기를 담은 socklen_t
​
	memset(&hints, 0, sizeof hints); // hints 구조체를 초기화
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
​
	// hints 구조체 정보를 바탕으로 ipaddr로 server 정보를 servinfo에 addrinfo를 얻어옴
	if ((status = getaddrinfo(ip_addr, MYPORT, &hints, &servinfo) != 0))
	{
		std::cout<< "getaddrinfo error: " << status << std::endl;
		exit(1);
	}
​
	// 소켓 유형을 지정하여 소켓을 생성하고 socket fd에 저장
	sock_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	// 소켓에는 두 주소가 할당되어야함 -> ip address, port number (IP로 컴퓨터를 특정, Port로 프로세스를 특정)
	// sockaddr(or sockaddr_in) 타입인 servinfo->ai_addr에 sin_port, sin_addr가 있어서 socket에 주소정보를 연결해줌
	bind(sock_fd, servinfo->ai_addr, servinfo->ai_addrlen);
	// 클라이언트에서 connect()가 올때까지 대기.. BACKLOG는 대기 큐의 크기; 지정한 fd가 리스닝 소켓이 됨
	listen(sock_fd, BACKLOG);
​
	// 대기 중인 클라이언트의 요청을 차례로 수락; accept의 반환값은 새로운 디스크립터 번호
	// 앞서 이용한 리스닝 소켓은 연결 요청을 대기시키는 과정까지를 담당하며,
	// accept() 함수를 통해 새로 할당받은 소켓으로 데이터 송수신을 할 수 있음
	// - sock : 서버소켓(리스닝소켓)의 디스크립터 번호
	// - addr : 대기 큐를 참조해 얻은 클라이언트의 주소정보
	// - addrlen : addr 변수의 크기
	// 새로운 소켓 디스크립터를 반환할 때에는 대기 큐에서 첫번째로 대기중인 연결요청을 참조
	// 대기 큐가 비어있는 상황이라면 새로운 요청이 올 때까지 accept값은 반환되지 않고 대기(blocking)
	addr_size = sizeof client_addr;
	new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &addr_size);
​
	// bind(), listen(), accept() 세가지 함수를 순차적으로 호출하고 나서 서버측에서의 데이터 송수신 가능
{
	const char	*msg = "silim was here!";
	int			len, bytes_sent;
​
	len = strlen(msg);
	bytes_sent = send(sock_fd, msg, len, 0);
}
	close(sock_fd);
	freeaddrinfo(servinfo);
	return (0);
}
​
​
int	wait_read(void)
{
	struct timeval	tv;
	fd_set			read_fds;
​
	tv.tv_sec = 10;
	tv.tv_usec = 500000;
​
	// read_fds 안에 모든 fd를 지워줌
	FD_ZERO(&read_fds);
	// STDIN을 read_fds에 연결
	FD_SET(STDIN, &read_fds);
​
	// write_fds나 그외의 fds는 여기서는 신경쓰지 않아도됨
	// 기존 데이터(fds)를 읽고 있는 도중 들어오는 연결(fd)을 기다리도록 감시
	select(STDIN + 1, &read_fds, NULL, NULL, &tv);
​
	if (FD_ISSET(STDIN, &read_fds))
		std::cout << "A key was pressed!" << std::endl;
	else
		std::cout << "Timed out." << std::endl;
​
	return 0;
}
​
void	*get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
​
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
​
int		main(void)
{
	fd_set	master; // master fd list // master set을 쓰는 이유는 select()가 받은 socket descriptor 묶음의 내용이 변하기 때문
	fd_set	read_fds; // select()를 위한 tmp fd
	int		fdmax; // max fd number
​
	int		listener; // 리스닝 fd
	int		new_fd; // 새로 accept()되는 socket fd
	struct sockaddr_storage	remote_addr; // client address
	socklen_t	addr_len;
​
	char	buf[256]; // client data를 위한 버퍼
	int		nbytes;
​
	char	remoteIP[INET6_ADDRSTRLEN];
​
	int		yes = 1; // 아래에서 setsockopt()을 SO_REUSEADDR로 설정하는 옵션을 위해...
	int		i, j, status;
​
	struct addrinfo	hints, *servinfo, *p;
​
	FD_ZERO(&master); // master를 비워주고 temp를 세팅
	FD_ZERO(&read_fds);
​
	// 소켓(fd)을 얻고 주소를 바인딩 시켜줌
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0)
	{
		throw std::runtime_error("addrinfo");
		exit(1);
	}
	for (p = servinfo; p != NULL; p = p->ai_next) // ai_next로 여러 인터넷 주소를 가질 수 있는 서버에 연결할 수도 있음
	{
		// 소켓 생성
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0)
			continue;
		// "address already in use" 에러 메세지
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
​
		// 소켓에 주소 정보를 바인딩 시켜줌
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(listener);
			continue;
		}
		break;
	}
​
	// servinfo가 없으면 바인딩할게 없음
	if (p == NULL)
	{
		throw std::runtime_error("bind");
		exit(2);
	}
​
	// 지정한 fd로 리스닝 소켓 생성
	if (listen(listener, 10) == -1)
	{
		throw std::runtime_error("listen");
		exit(3);
	}
​
	// master set에 리스닝 소켓 추가 // listener socket이 데이터를 읽을 준비가 되었다면, 이는 새롭게 들어와서 기다리고 있는 연결이 있음
	FD_SET(listener, &master);
​
	// 가장 큰 file descriptor 값을 따라감
	fdmax = listener; // 지금은 listener가 해당
​
	// 메인 루프
	while (1)
	{
		// read_fds에 master를 복사해서 select()를 호출
		read_fds = master; // 새로운 연결이 생기거나 끊길 때마다, 이는 master 묶음에 반영
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
		{
			throw std::runtime_error("select");
			exit(4);
		}
​
		// 데이터를 읽기 위해 기존에 해둔 연결들을 실행
		for (i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i, &read_fds)) // fd가 read_fds에 있으면
			{
				if (i == listener) // 새로운 연결(listener)이라면
				{
					addr_len = sizeof remote_addr;
					// socket()으로 만든 end-point 소켓 fd인 listener에 연결이 들어오는지 대기
						// 대기 중인 클라이언트의 요청을 차례로 수락; accept의 반환값은 새로운 디스크립터 번호
						// 앞서 이용한 리스닝 소켓은 연결 요청을 대기시키는 과정까지를 담당하며,
						// accept() 함수를 통해 새로 할당받은 소켓으로 데이터 송수신을 할 수 있음
						// - sock : 서버소켓(리스닝소켓)의 디스크립터 번호
						// - addr : 대기 큐를 참조해 얻은 클라이언트의 주소정보
						// - addrlen : addr 변수의 크기
						// 새로운 소켓 디스크립터를 반환할 때에는 대기 큐에서 첫번째로 대기중인 연결요청을 참조
						// 대기 큐가 비어있는 상황이라면 새로운 요청이 올 때까지 accept값은 반환되지 않고 대기(blocking)
					new_fd = accept(listener, (struct sockaddr *)&remote_addr, &addr_len); // accept()하고 master set에 추가
​
					if (new_fd == -1) // new_fd가 없으면 accept 할게 없음
						throw std::runtime_error("accept");
					else
					{
						FD_SET(new_fd, &master); // new_fd를 master에 추가시켜줌
						if (new_fd > fdmax)
							fdmax = new_fd;
					}
					std::cout << "selectserver: new connection on socket " << new_fd << std::endl;
				} // 기존에 있던 연결이라면
				else
				{
					// 클라이언트로부터 데이터를 받음
					if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
					{
						if (nbytes == 0) // recv() 반환값이 0이라면 이는 클라이언트에서 접속을 끊은 것이므로 master에서도 빼줌
							std::cout << "selectserver: socket " << i << " hung up" << std::endl;
						else
							throw std::runtime_error("recv");
						close(i); // 연결 종료
						FD_CLR(i, &master); // master set에서 지워줌
					}
					else // recv() 반환값이 0이 아니라면 서버가 client로부터 실제로 데이터를 받았음
					{
						for (j = 0; j <= fdmax; j++) // master set을 돌면서 연결되어 있는 나머지 클라이언트에게 그 데이터를 전달
						{
							// 모두에게 버퍼를 전달 (다대다 채팅 서버)
							if (FD_ISSET(j, &master))
							{
								if (j != listener && j != i)
									if (send(j, buf, nbytes, 0) == -1)
										throw std::runtime_error("send");
							}
						}
					}
				} // END 클라이언트로부터 데이터 핸들링
			} // END 새로운 연결(listener)이 있는지 확인
		} // END 데이터를 읽기 위해 파일 디스크립터를 통해 루프
	} // END while(1) -- 무한 루프 -> 서버가 계속 열려있음
​
	return 0;
}
