#include <ESP8266WiFi.h>

WiFiServer server(80);
void setup()
{
  Serial.begin(115200);
  pinMode(0, OUTPUT);   //gpi 0 como saida
  digitalWrite(0, LOW); // pre set do valor como LOW

  Serial.print("Conectando....");

  WiFi.begin("Copel-33", "12017330"); // conecxao com o wifi de casa

  //Esperando conectar
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.print("");
  Serial.print("Conectou!");

  //Configurações do IP fixo.
  IPAddress ip(192, 168, 100, 10);
  IPAddress gateway(192, 168, 100, 10);
  IPAddress subnet(255, 255, 255, 0);
  Serial.print("Configurando IP fixo para : ");
  Serial.println(ip);
  WiFi.config(ip, gateway, subnet); // Configura no roteador

  // inicializa o servidor
  server.begin();
  // verificaçao do ip que foi iniciado pelo serial monitor
  Serial.print("Server em: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  WiFiClient client = server.available(); // verifica se alguem se conectou
  if (!client)
  {
    // se nao tiver client retorna pois nao tem o que fazer
    delay(150);
    return;
  }

  Serial.println("Algum maluco no pedaco");

  String req = client.readStringUntil('\r');
  Serial.println("Requisicao");
  Serial.println(req);

  String html =
      "<html>"
      "<head>"
      "<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>"
      "<title>ESP8266</title>"
      "<style>"
      "body{"
      "background-color:#515151;"
      "text-align: center;"
      "font-family: sans-serif;"
      "font-size:14px;"
      "padding: 25px;"
      "}"

      "p{"
      "color:#444;"
      "}"

      "button{"
      "outline: none;"
      "border: 2px solid #1fa3ec;"
      "border-radius:18px;"
      "background-color:#515151;"
      "color: #1fa3ec;"
      "padding: 10px 50px;"
      "}"

      "button:active{"
      "color: #fff;"
      "background-color:#1fa3ec;"
      "}"
      "</style>"
      "</head>"
      "<body>"

      "<p><a href='?acao=gpio0On'><button>LUMOS</button></a></p>"
      "<p><a href='?acao=gpio0Off'><button>NOX</button></a></p>"

      "</body>"
      "</html>";

  client.print(html); //coloca o html no buffer que vai ser enviado pro maluco que se conectou

  client.flush(); // envia os dados pro client

  // verifica se a requisicao manda LUMOS
  if (req.indexOf("acao=gpio0On") != -1)
  {
    digitalWrite(0, HIGH);
  }
  else if (req.indexOf("acao=gpio0Off") != -1)
  {
    digitalWrite(0, LOW);
  }

  delay(2000);

  //fecha conexao com o maluco - padrao da comunicacao ter que feechar a bagaca
  client.stop();
  Serial.println("Cliente desconectado");
}
