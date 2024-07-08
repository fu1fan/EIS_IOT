<template>
    <div>
        <el-card v-for="index in battery_count" :key="index">
            <!-- Card content here -->
            <div>
                <p>Battery {{ index }}</p>
                <p>100Hz阻抗: {{ ohmages[index - 1] }} （大小与电池驱动能力成强负相关）</p>
                <p v-if="ohmages[index - 1] > 20" class="warning">动力不足</p>
                <p v-if="ohmages[index - 1] == 0" class="warning">阻抗过大，超出量程，请送检</p>
            </div>
        </el-card>
    </div>
</template>

<script setup>
import { ref, onMounted, onUnmounted } from 'vue'

let battery_count = ref(0)
let ohmages = ref([])
let voltages = ref([])
let last_update = ref('')

let intervalId = null;

onMounted(() => {
  intervalId = setInterval(() => {
    fetch('/api/c/is_online')
      .then(response => response.json())
      .then(data => {
        if (data.data == true) {
          fetch('/api/c/get_state')
            .then(response => response.json())
            .then(data => {
              if (data.status == "success") {
                battery_count.value = data.data.battery_count
                voltages.value = data.data.voltages_cur
                ohmages.value = data.data.ohmages
                last_update.value = new Date(data.data.last_update * 1000).toLocaleTimeString()
              }
            })
            .catch(error => {
              console.error('Error:', error);
            });
        } else {
          console.log('设备离线');
        }
      })
      .catch(error => {
        console.error('Error:', error);
      });
  }, 2000);
});

onUnmounted(() => {
  clearInterval(intervalId);
});
</script>

<style scoped>
.container {
    display: flex;
    flex-wrap: wrap;
    justify-content: space-between;
}

.card {
    width: 45%;
    margin-bottom: 20px;
}

.card-content {
    padding: 10px;
}

.card-title {
    font-weight: bold;
    margin-bottom: 5px;
}

.card-voltage {
    margin-bottom: 5px;
}
</style>
