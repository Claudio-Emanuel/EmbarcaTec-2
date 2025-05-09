# Importa as bibliotecas Flask e SocketIO
from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit

# Cria a instância do Flask
app = Flask(__name__)

# Configura o SocketIO para permitir conexões de qualquer origem
socketio = SocketIO(app, cors_allowed_origins="*")

# Rota principal que serve a página HTML
@app.route('/')
def index():
    return render_template('index.html')  # Renderiza o template web/templates/index.html

@app.route('/CLICK_A', methods=['GET', 'POST'])
def click_a():
    print("Comando: Click")
    socketio.emit('command', {'action': 'click_a'})  # Envia comando para ON
    return 'Click_a command sent', 200

@app.route('/CLICK_B', methods=['GET', 'POST'])
def click_b():
    print("Comando: Click")
    socketio.emit('command', {'action': 'click_b'})  # Envia comando para ON
    return 'Click_b command sent', 200

@app.route('/SOLTO_A', methods=['GET', 'POST'])
def solto_a():
    print("Comando: solto")
    socketio.emit('command', {'action': 'solto_a'})  # Envia comando para OFF
    return 'solto_a command sent', 200

@app.route('/SOLTO_B', methods=['GET', 'POST'])
def solto_b():
    print("Comando: solto_B")
    socketio.emit('command', {'action': 'solto_b'})  # Envia comando para OFF
    return 'solto_b command sent', 200

@app.route('/CENTRO', methods=['GET', 'POST'])
def centro():
    print("Joystick: CENTRO")
    socketio.emit('command', {'action': 'centro'})
    return 'CENTRO command sent', 200

@app.route('/NORTE', methods=['GET', 'POST'])
def norte():
    print("Joystick: NORTE")
    socketio.emit('command', {'action': 'norte'})
    return 'NORTE command sent', 200

@app.route('/SUL', methods=['GET', 'POST'])
def sul():
    print("Joystick: SUL")
    socketio.emit('command', {'action': 'sul'})
    return 'SUL command sent', 200

@app.route('/LESTE', methods=['GET', 'POST'])
def leste():
    print("Joystick: LESTE")
    socketio.emit('command', {'action': 'leste'})
    return 'LESTE command sent', 200

@app.route('/OESTE', methods=['GET', 'POST'])
def oeste():
    print("Joystick: OESTE")
    socketio.emit('command', {'action': 'oeste'})
    return 'OESTE command sent', 200

@app.route('/NORDESTE', methods=['GET', 'POST'])
def nordeste():
    print("Joystick: NORDESTE")
    socketio.emit('command', {'action': 'nordeste'})
    return 'NORDESTE command sent', 200

@app.route('/SUDESTE', methods=['GET', 'POST'])
def sudeste():
    print("Joystick: SUDESTE")
    socketio.emit('command', {'action': 'sudeste'})
    return 'SUDESTE command sent', 200

@app.route('/NOROESTE', methods=['GET', 'POST'])
def noroeste():
    print("Joystick: NOROESTE")
    socketio.emit('command', {'action': 'noroeste'})
    return 'NOROESTE command sent', 200

@app.route('/SUDOESTE', methods=['GET', 'POST'])
def sudoeste():
    print("Joystick: SUDOESTE")
    socketio.emit('command', {'action': 'sudoeste'})
    return 'SUDOESTE command sent', 200


# Ponto de entrada principal da aplicação
if __name__ == '__main__':
    # Inicia o servidor Flask com suporte a WebSockets
    socketio.run(app, host='0.0.0.0', port=5000)
