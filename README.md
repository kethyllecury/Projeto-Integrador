# Projeto Integrador

## Introdução

A problemática central deste projeto parte da constatação de que pessoas com deficiência ou que possuem direito ao acesso livre muitas vezes não se sentem seguras e confiantes ao utilizar o transporte público. Atualmente, não existe uma forma visível e imediata que garanta que seu direito será reconhecido no momento do embarque. Motoristas nem sempre conseguem identificar rapidamente esses passageiros, o que pode causar constrangimento, atrasos operacionais ou até mesmo impedir que esses cidadãos utilizem o transporte com autonomia e dignidade.

Nossa solução busca automatizar essa identificação, tornando o processo mais confortável tanto para a pessoa com deficiência quanto para o motorista. Ao ler o cartão do passageiro, o sistema notifica o motorista instantaneamente por meio de um alerta visual na parada, informando rapidamente as necessidades da pessoa com deficiência. O projeto é inclusivo e melhora a experiência no transporte público, desde a espera no ponto (com informações sobre tempo de chegada e linha do veículo) até o embarque, contando ainda com um sinal sonoro para auxiliar pessoas com deficiência visual na localização do dispositivo.

## Objetivos

### Objetivo Geral

Investigar como a tecnologia IoT pode garantir o reconhecimento de passageiros com direito ao acesso livre no transporte público, informar em tempo real sobre a chegada e a linha dos veículos, e propor um sistema replicável de identificação automática para motoristas através de alertas visuais e sonoros.

## Tecnologias Utilizadas

- Internet das Coisas (IoT) ESP32C3
- Alertas visuais e sonoros (Alexa)
- Render
- MongoDB Atlas
- Integração com APIs de transporte público (Google Maps API)

## Como Funciona

1. O passageiro aproxima o cartão ao leitor do dispositivo.
2. O sistema identifica automaticamente o direito de acesso.
3. O motorista percebe um alerta visual imediato no painel da parada de ônibus.
4. Um sinal sonoro é emitido para auxiliar pessoas com deficiência visual.
5. Informações sobre tempo de chegada e linha do veículo são disponibilizadas ao passageiro.

## Estrutura do Projeto

```
Projeto-Integrador/
├── alexa/alexa command.py
├── dashboard/DashIOT.pbix
├── iot/esp32_rfid.ino
├── scripts/api_maps.py
├── README.md
├── main.py
└── requirements.txt
```

## Contribuição

Contribuições são bem-vindas! Sinta-se à vontade para abrir issues ou enviar pull requests.

1. Faça um fork do projeto
2. Crie uma branch (`git checkout -b feature/NovaFuncionalidade`)
3. Commit suas alterações (`git commit -m 'Adiciona nova funcionalidade'`)
4. Push para a branch (`git push origin feature/NovaFuncionalidade`)
5. Abra um Pull Request

## Licença

Este projeto está sob a licença [MIT](LICENSE).

---

**Dúvidas ou sugestões?**  
Entre em contato via [Issues](https://github.com/kethyllecury/Projeto-Integrador/issues).
