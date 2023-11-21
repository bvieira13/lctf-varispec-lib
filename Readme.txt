Readme

O controle do módulo VariSpec VIS-07-HC-20 pode ser feito de 3 formas distintas. A primeira é utilizando-se de um SDK com bibliotecas estáticas (.lib) e dinâmicas (.dll) presentes no arquivo de instalção do fabricante (VsRev1p37.exe) e voltada principalmente para aplicações em C/C++ em Windows. A outra forma seria a partir de bibliotecas desenvolvidas para softwares parceiros como LabView e o MatLab. Por fim, pode-se também fazer o controle desse dispositivo utilizando recursos de comunicação serial a partir de comandos tabelados e listados pelo fabricante no manual do usuário.

Das formas de controle listadas acima, estudou-se como utilizar tanto o SDK como a comunicação serial para desempenhar o controle do dispositivo. 

O primeiro comando testado de porta serial foi de status e busy, que servem como forma de monitorar o comportamento dodispoitivo em termos de erro ou se o mesmo ainda está ocupado com a algum outro comando antes de prosseguir no código.

Durante os testes, verificou-se que o comando de read da biblioteca pySerial para receber 2 bytes demorava mais entre 20 a 4 ms, o que poderia ser um problema. Verificou-se no stack overflow (https://www.projectgus.com/2011/10/notes-on-ftdi-latency-with-arduino/) que isso estava relacionado com a latência do controlador FTDI (conversor de USB para UART) que estava configurado para ser mais ou menos 16 ms, dessa forma alterou-se essa configuração seguindo o passo a passo estupulado em https://github.com/ros-drivers/rosserial/issues/383.

O módulo VariSpec VIS-07-HC-20 pode responder em dois baudrate distintos (9600 e 115200 kbps) no entanto tal alteração deve ser feita abrindo o equipamento e alterando um jumper interno do mesmo.