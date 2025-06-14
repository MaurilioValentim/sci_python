# 📡 sci_python

Projeto em Python para comunicação serial com o microcontrolador **TMS320F28379D** via interface SCI.  
Permite **enviar** e **receber** números inteiros com base em um protocolo simples, compatível com o firmware embarcado no DSP.

---

## 🧰 Requisitos

- Python 3.8 ou superior
- Visual Studio Code (VSCode)
- Extensão Python instalada no VSCode
- Firmware SCI já programado no TMS320F28379D
- Driver da porta COM (XDS / FTDI etc.)

---

### 1. Obter o Projeto (📌 Faça o Fork)

Antes de clonar, você deve **fazer um fork** deste repositório para a sua conta do GitHub:

1. Acesse o repositório original no GitHub:  
   👉 [`https://github.com/Pguilhermem/sci_python`](https://github.com/Pguilhermem/sci_python)

2. Clique no botão `Fork` no canto superior direito da página.

3. Escolha sua conta como destino do fork.

4. Após o fork, vá até **o repositório copiado na sua conta** e copie o link de clonagem (HTTPS ou SSH).

5. No seu terminal, execute:

```bash
git clone https://github.com/seuusuario/sci_python.git
cd sci_python
```

Substitua `seuusuario` pelo seu nome de usuário no GitHub.

---

### 2. Abrir o Projeto no VSCode

Abra a pasta do projeto no VSCode:

- `Arquivo > Abrir Pasta...`  
- Selecione a pasta onde está o script `sci_python.py`

---

### 3. Criar Ambiente Virtual

Você pode criar o ambiente **graficamente ou pelo terminal**:

#### ✅ Opção 1: Pelo VSCode (modo gráfico)

1. Clique na aba inferior onde aparece o número da versão do Python (canto inferior direito do VSCode).
2. Uma lista de ambientes será exibida. Clique em **"Criar Ambiente"** ou selecione **Python: Create Environment**.
3. Escolha a opção **Venv** e aguarde a criação do ambiente virtual `.venv`.

![Criação do ambiente virtual]([images\VSCodePrint.png](https://github.com/Pguilhermem/sci_python/blob/main/images/VSCodePrint.png))

#### 🧪 Opção 2: Pelo terminal (modo manual)

Abra o terminal do VSCode:

- Menu: `Terminal > Novo Terminal`
- Ou atalho: `Ctrl + ` (Ctrl + acento grave)

E execute:

```bash
python -m venv .venv
```

Ative o ambiente virtual:

- **Windows (cmd):**
  ```bash
  .venv\Scripts\activate
  ```

- **PowerShell:**
  ```bash
  .venv\Scripts\Activate.ps1
  ```

- **Linux/macOS:**
  ```bash
  source .venv/bin/activate
  ```

---

### 4. Selecionar o Interpretador Python

Após a criação do ambiente virtual:

- Clique novamente no **número da versão do Python** no canto inferior direito do VSCode.
- Selecione o Python localizado em `.venv`

📸 *[Adicione outro print aqui, se desejar, mostrando a lista de intérpretes]*

---

### 5. Instalar Dependências

> **⚠️ Esta etapa deve ser feita com o terminal aberto e o ambiente virtual ativado.**

1. Abra o terminal no VSCode:  
   - Menu: `Terminal > Novo Terminal`  
   - Ou atalho: `Ctrl + `

2. Com o ambiente virtual ativo (deve aparecer algo como `(.venv)` no terminal), instale a biblioteca:

```bash
pip install pyserial
```

Se quiser congelar as dependências em um arquivo (opcional):

```bash
pip freeze > requirements.txt
```

---

### 6. Configurar a Porta Serial

Edite a variável `SERIAL_PORT` no início do código:

```python
SERIAL_PORT = 'COM4'  # Altere para a porta COM do seu dispositivo
```

No Windows, consulte a porta no **Gerenciador de Dispositivos > Portas (COM e LPT)**.

---

### 7. Executar o Script

No terminal (com o ambiente virtual ativado), execute:

```bash
python sci_python.py
```

---

## 💻 Interface do Programa

Ao iniciar, o terminal exibirá o seguinte menu:

```
----- MENU -----
1. Enviar um numero inteiro para o 28379D
2. Receber um numero inteiro do 28379D
0. Sair
```

---

## ❗ Problemas Comuns

- **Porta COM incorreta:** Verifique no Gerenciador de Dispositivos.
- **Timeout:** Certifique-se de que o 28379D está ligado e com firmware SCI funcional.
- **Permissão (Linux):** Pode ser necessário rodar `sudo usermod -a -G dialout $USER`.

---

## 📄 Licença

Este projeto é livre para fins educacionais e de testes com o TMS320F28379D.

---
