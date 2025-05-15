// Ganti dengan alamat WebSocket broker EMQX-mu
const brokerUrl = 'wss://broker.emqx.io:8084/mqtt'; // bisa juga ws://YOUR_IP:8083/mqtt
const clientId = 'web-client-' + Math.random().toString(16).substr(2, 8);
const options = {
    clientId: clientId,
    clean: true,
    connectTimeout: 4000,
    username: '', // jika ada
    password: '', // jika ada
}
// Handler saat konek/error
// Menampilkan log jika berhasil konek ke broker atau jika gagal/error.
const client = mqtt.connect(brokerUrl, options);
client.on('connect', () => {
    console.log('Tersambung ke EMQX Broker');
    // kita subscribe ke topik status untuk lihat esp32 on atau off
    // status mqtt broker
    let brokerStatus = document.getElementById("status_mqtt");
    brokerStatus.innerHTML = "ONLINE";
    brokerStatus.style.color = "green";
    console.log('Koneksi MQTT berhasil');
    client.subscribe("switch/Status/#", { qos: 1 }, function(err) {
        if (!err) {
            console.log('Subscribe ke topic kelasiot/# berhasil');
        } else {
            console.error('Gagal subscribe ke topic kelasiot/#', err);
        }
    });
});
client.on('error', (err) => {
    console.error('MQTT Error:', err);
    //status mqtt broker
    let brokerStatus = document.getElementById("status_mqtt");
    brokerStatus.innerHTML = "OFFLINE";
    brokerStatus.style.color = "red";
});
 // Fungsi untuk publish status lampu
function updateStatusSwitch(id, status) {
    const topic = `switch/${id}/status`;
    client.publish(topic, status, { qos: 1, retain:false}, (err) => {
        if(err) {
            console.error(`Gagal kirim ke ${topic}:`, err);
        } else{
            console.log(`Status ${status} dikirim ke ${topic}`);
        }
    });
}
// ubah tulisan esp32
client.on('message', function(topicx, messagex) {
    const payload = messagex.toString();

    if (topicx === "switch/Status/POPO123") {
        const brokerStatus = document.getElementById("statusserverz");
        
        if (payload === "ONLINE") {
            brokerStatus.innerHTML = "ONLINE";
            brokerStatus.style.color = "green";
        } else if (payload === "OFFLINE") {
            brokerStatus.innerHTML = "OFFLINE";
            brokerStatus.style.color = "red";
        }

        console.log("Status ESP32: " + payload);
    }
});

// untuk ubah warna icon
const switchLampu1 = document.getElementById('switch-lampu-1');
const iconLampu1 = document.getElementById('icon-lampu-1');
const switchLampu2 = document.getElementById('switch-lampu-2');
const iconLampu2 = document.getElementById('icon-lampu-2');
const switchLampu3 = document.getElementById('switch-lampu-3');
const iconLampu3 = document.getElementById('icon-lampu-3');
const switchLampu4 = document.getElementById('switch-lampu-4');
const iconLampu4 = document.getElementById('icon-lampu-4');

switchLampu1.addEventListener('change', function() {
    // declare dulu variable status
    const status = this.checked ? 'ON' : 'OFF';
    // Update icon tampilan
    iconLampu1.classList.toggle('icon-active', status === 'ON'); //selain mengubah tampilan dengan mengambil class css, ini juga mengubah nilai status
    iconLampu1.classList.toggle('icon-inactive', status === 'OFF');
    // Panggil fungsi update status
    updateStatusSwitch(1, status); //id = 1 untuk switch 1 , nilai parameter status diambil dari iconLampu1.classList.toggle
});
switchLampu2.addEventListener('change', function() {
    const status = this.checked ? 'ON' : 'OFF';
    // Update icon tampilan
    iconLampu2.classList.toggle('icon-active2', status === 'ON');
    iconLampu2.classList.toggle('icon-inactive', status === 'OFF');
    updateStatusSwitch(2, status);
});
switchLampu3.addEventListener('change', function() {
    const status = this.checked ? 'ON' : 'OFF';
    // Update icon tampilan
    iconLampu3.classList.toggle('icon-active3', status === 'ON');
    iconLampu3.classList.toggle('icon-inactive', status === 'OFF');
    updateStatusSwitch(3, status);
});
switchLampu4.addEventListener('change', function() {
    const status = this.checked ? 'ON' : 'OFF';
    // Update icon tampilan
    iconLampu4.classList.toggle('icon-active4', status === 'ON');
    iconLampu4.classList.toggle('icon-inactive', status === 'OFF');
    updateStatusSwitch(4, status);
});


