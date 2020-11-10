#define DIST_SENSOR_TRIGGER 3
#define DIST_SENSOR_ECHO 4
#define DIST_SENSOR_MAX 23324 //Limite de espera máximo do pulso, 23324us ~= 400 cm
#define DIST_SENSOR_NUM_MEDIDAS 10 //Num de medidas no esquema de buffer circular
#define PINO_LED_PERTO 2
 
float medidas[DIST_SENSOR_NUM_MEDIDAS]; //Buffer de medidas
float media; //Resultado da media do buffer
int indice; //indice a ser usado no apontamento ao buffer
 
void distanceSensorInit(){ //Prepara(inicializa) para sensor de distância
  pinMode(DIST_SENSOR_TRIGGER, OUTPUT); //Configurações dos pinos
  pinMode(DIST_SENSOR_ECHO, INPUT);
  digitalWrite(DIST_SENSOR_TRIGGER, LOW);
}
 
float distanceSensorRead(){ //Realiza uma leitura de distância do sensor
  digitalWrite(DIST_SENSOR_TRIGGER, HIGH); //Pulso alto de 10us
  delayMicroseconds(10);
  digitalWrite(DIST_SENSOR_TRIGGER, LOW);
  unsigned long duracao;
  duracao = pulseIn(DIST_SENSOR_ECHO, HIGH, DIST_SENSOR_MAX); //Le a duracao do pulso alto no ECHO
  if(duracao==0){ //se pulseIn retornou 0 = atingiu o limite
    return -1;
  }
  else{ //senao = distancia valida
    float distancia;
    distancia = (0.0343 / 2) * duracao;
    return distancia;
  }
}
 
void setup() {
  Serial.begin(19200);
  distanceSensorInit();
  pinMode(PINO_LED_PERTO, OUTPUT);
  digitalWrite(PINO_LED_PERTO, LOW);
  indice=0;
  medidas[DIST_SENSOR_NUM_MEDIDAS-1]=-1; //Inicializa a ultima posicao do vetor como -1 que representa medida invalida
}
 
void loop() {
  medidas[indice] = distanceSensorRead(); //realiza medida e armazena no vetor do buffer
  indice = (indice + 1) % DIST_SENSOR_NUM_MEDIDAS; //incrementa o indice (e realiza o retorno ao 0 automaticamente)
 
  media = 0; //calculo da media
  for(int i=0; i<DIST_SENSOR_NUM_MEDIDAS; i++){
	if(medidas[i]<0){ //se a medida for invalida
	  Serial.println("Medida invalida");
	  return; //finaliza a execucao do loop
	}
    media += medidas[i];
  }
  media /= DIST_SENSOR_NUM_MEDIDAS;
  Serial.println(media);
 
  if(media < 20){ //se media dos valores no buff for menor que 20 (ou seja, há um objeto próximo)
    digitalWrite(PINO_LED_PERTO, HIGH);
  }
  else{
    digitalWrite(PINO_LED_PERTO, LOW);
  }
}