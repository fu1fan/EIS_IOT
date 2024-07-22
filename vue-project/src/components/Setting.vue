<template>
  <el-row gutter=20>
    <el-col :span=14>
      <div style="display: flex; flex-direction: column; height: 100%; padding: 10px;">
        <el-space direction="vertical" alignment="flex-start" :size=20 style="width: 100%;" fill>
          <div style="display: flex; justify-content: space-between; align-items: center;">
            <h1>邮件设置</h1>
            <el-space>
              <el-text>开启邮件通知</el-text>
              <el-switch v-model="enable_mail" @change="set_mail">启用邮件通知</el-switch>
            </el-space>
          </div>
          <div style="margin-left: 5px;">
            <el-text>收件邮箱：</el-text>
            <el-input v-model="mail_to" placeholder="收件人邮箱地址" style="margin-top: 5px;" />
          </div>
          <el-button type="primary" @click="set_mail_to">保存</el-button>
        </el-space>
        <div class="center">
          <el-text class="hardToNameIt">时间有限，止步于此~</el-text>
        </div>
      </div>
    </el-col>
    <el-col :span=10>
      <el-card shadow="hover" style="display: flex; flex-direction: column; height: 100%;" body-style="flex-grow: 1;">
        <el-space direction="vertical" alignment="flex-start" :size=20 style="width: 100%;" fill>
          <div style="display: flex; justify-content: space-between; align-items: center;">
            <h1>关于</h1>
            <el-text type="info">2024嵌入式设计大赛 · Loong的传人</el-text>
          </div>
          <div style="margin-left: 5px;">
          </div>
          <div>
            <el-space direction="vertical" alignment="flex-start">
              <el-text>从博士生手中拿到EIS课题开始，我们花费了三个多月的时间，从硬件搭建、调试再到嵌入式开发、服务器程序编写、前端开发，最终成为了今天的样子。</el-text>
              <el-text>然而任何一个完整项目的开发都不可能一帆风顺的，</el-text>
              <el-text>依稀还记得花重金购买5片AD835乘法器做双相关检测，结果发现线性度不满足要求时的沮丧；又因不小心把一块100多的动力电池过放报废、半瓶可乐倒在作品上而觉得好笑。</el-text>
              <el-text>很可惜，由于这是我们第一次接触有源器件的参数测量，没有太多参考，连测量方案都是我们慢慢摸索出来的，导致我们没有时间把所有电路设计到同一张电路板上，只好选择更方便调试的模块化方案，让成品显得有些零乱。</el-text>
              <el-text>但不管怎样，我们会永远记得第一次在Excel上画出测量到的EIS曲线时的激动，第一次完成网页端远程测量的自豪。无论最终获得什么名次，对于我们来说，此时此刻，<b>轻舟已过万重山。</b></el-text>
              <!-- <el-text>最让我破防的还是学校的实训老师谢某某，拜他所赐，这个项目近万行代码几乎有一般是我熬夜到两点多写的，第二天早八还要参加AD10的理论课，我真想知道还有哪些公司在用这个自动布线都能卡死的古董软件。</el-text> -->
              <!-- <el-text>多的也不说了，还是要感谢这次比赛，让我有机会在大一就体验到了写论文致谢的感觉（笑）。</el-text> -->
              <el-text id="yeah">最后，我们很自豪的是，这套作品的完成没有划水、没有学长老师的代劳、没有继承，是我们自己从零赋予了它生命。我们，是创造者。</el-text>
            </el-space>
          </div>
        </el-space>
      </el-card>
    </el-col>
  </el-row>
  <div style="display: flex; justify-content: space-between; align-items: center;">
    <span>
      <el-space>
        <el-tag type="primary">Vue.js</el-tag>
        <el-tag type="primary">Element-Plus</el-tag>
        <el-tag type="primary">Python</el-tag>
        <el-tag type="primary">Flask</el-tag>
        <el-tag type="success">...</el-tag>
      </el-space>
    </span>
    <!-- <br /> -->
    <div style="display: flex; justify-content: flex-end;"><el-space><a>Presented by</a><a
      href="https://github.com/fu1fan/" id="link">fu1fan</a></el-space></div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'

const enable_mail = ref(false)
const mail_to = ref('')

// @app.route('/api/c/enable_mail')
// def c_enable_mail():
//     global enable_mail
//     enable_mail = True
//     return {"status": "success"}

// @app.route('/api/c/disable_mail')
// def c_disable_mail():
//     global enable_mail
//     enable_mail = False
//     return {"status": "success"}

// @app.route('/api/c/set_mail_to', methods=['POST'])
// def c_set_mail_to():
//     global poster
//     data = request.get_json()
//     if "to" not in data:
//         return {"status": "error", "message": "Invalid data"}
//     if "to" not in data or not isinstance(data["to"], str) or not re.match(r"[^@]+@[^@]+\.[^@]+", data["to"]):
//         return {"status": "error", "message": "Invalid email address"}
//     poster.to = data["to"]
//     return {"status": "success"}

onMounted(() => {
  fetch('/api/c/get_mail_setting')
    .then(res => res.json())
    .then(data => {
      enable_mail.value = data.enable
      mail_to.value = data.to
    })
})

function set_mail_to() {
  fetch('/api/c/set_mail_to', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({ to: mail_to.value })
  })
    .then(res => res.json())
    .then(data => {
      if (data.status === 'success') {
        alert('设置成功')
      } else {
        alert('设置失败')
      }
    })
}

function set_mail() {
  if (enable_mail.value) {
    fetch('/api/c/enable_mail')
      .then(res => res.json())
      .then(data => {
        if (data.status === 'success') {
        } else {
          alert('设置失败')
        }
      })
  } else {
    fetch('/api/c/disable_mail')
      .then(res => res.json())
      .then(data => {
        if (data.status === 'success') {
        } else {
          alert('设置失败')
        }
      })
  }
}


</script>

<style scoped>
h1 {
  font-size: large;
}

b {
  font-weight: bold;

}

#link {
  color: #6095cc;
  font-weight: bold;
  text-decoration: none;
}

#yeah {
  color: #004b9a;
  font-weight: bold;
}

.center {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  height: 100%;
}

.hardToNameIt {
  font-size: 20px;
  opacity: 0.2;
}
</style>
