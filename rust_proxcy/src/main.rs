use tokio::net::{TcpListener, TcpStream};

#[tokio::main]
async fn main() -> std::io::Result<()> {
    let listener = TcpListener::bind("127.0.0.1:25565").await?;

    loop {
        let (mut client, _) = listener.accept().await?;

        tokio::spawn(async move {
            let mut server = match TcpStream::connect("127.0.0.1:25566").await {
                Ok(server) => server,
                Err(error) => {
                    eprintln!("Could not connect to Minecraft server: {error}");
                    return;
                }
            };

            match tokio::io::copy_bidirectional(&mut client, &mut server).await {
                Ok((client_byters, server_bytes)) => {
                    if client_byters > 0 {
                        println!("client -> Server: {} bytes", client_byters);
                    }
                    if server_bytes > 0 {
                        println!("Server -> client: {} bytes", server_bytes);
                    }
                }
                Err(error) => eprintln!("Proxy error: {error}"),
            }
        });
    }
}
