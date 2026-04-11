
/* ステップ１：　ソケットを作成する*/
  if ((sock_id = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    oops("socket");

  /** ステップ2 : サーバーに接続する*/

  memset(&servadd, 0, sizeof(servadd));
  hp = gethostbyname(av[1]);
  if (hp == NULL)
    oops(av[1]);

  memcpy((struct sockaddr *)&servadd.sin_addr, hp->h_addr, hp->h_length);
  servadd.sin_port = htons(PORTNUM);
  servadd.sin_family = AF_INET;

  if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
    oops("connect");
