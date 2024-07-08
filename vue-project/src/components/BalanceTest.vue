<template>
    <div>
        <el-card v-for="index in battery_count" :key="index">
            <!-- Card content here -->
            <div>
                <p>Battery {{ index + 1 }}</p>
                <p>Voltage: {{ voltages[index] }}</p>
            </div>
        </el-card>
    </div>
</template>

<script setup>
let battery_count = ref(0)
let ohmages = ref([])
let voltages = ref([])
let last_update = ref('')

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
