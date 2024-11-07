while True:
    import paho.mqtt.client as mqtt

    # Variável global para armazenar o valor do tópico
    topic_value = None

    # Função chamada quando uma mensagem é recebida
    def on_message(client, userdata, msg):
        global topic_value
        topic_value = msg.payload.decode()  # Salva o valor na variável
        print(f"Valor recebido: {topic_value}")

    # Configurações do cliente MQTT
    client = mqtt.Client()

    # Configura a função de callback para quando uma mensagem for recebida
    client.on_message = on_message

    # Conecte ao broker MQTT (substitua pelo endereço e porta corretos)
    client.connect("broker.hivemq.com", 1883)

    # Inscreva-se em um tópico (substitua pelo tópico correto)
    client.subscribe("SENSOR/ULTRASSOM")

    # Inicie o loop para receber as mensagens
    client.loop_start()

    # Aguarde até que uma mensagem seja recebida e a variável seja preenchida
    while topic_value is None:
        pass  # Fica em loop até que o valor do tópico seja recebido

    print(f"Valor final do tópico: {topic_value}")
