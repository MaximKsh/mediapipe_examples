package com.mediapipe_demonstration.inference

import android.os.Bundle
import android.widget.TextView
import android.widget.Button
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity
import com.google.mediapipe.framework.Graph
import com.google.mediapipe.framework.AndroidPacketCreator
import android.graphics.BitmapFactory
import com.google.mediapipe.framework.PacketGetter
import java.io.BufferedInputStream
import java.io.InputStream
import java.net.HttpURLConnection
import java.net.URL
import android.content.Intent
import com.google.mediapipe.framework.AndroidAssetUtil

// bazel-2.0.0 mobile-install --start_app -c opt --config=android_arm64 //inference/android/src/main/java/com/mediapipe_demonstration/inference:Inference
class MainActivity : AppCompatActivity() {
    val PICK_IMAGE = 1
    var mpGraph: Graph? = null
    var timestamp = 0L
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        this.setContentView(R.layout.activity_main)
        val outputTv = findViewById<TextView>(R.id.outputTv)
        val button = findViewById<Button>(R.id.selectButton)
        AndroidAssetUtil.initializeNativeAssetManager(this)
        // Загрузка и инициализация графа
        // В данном случае граф преобразуется в бинарный формат
        val graph = Graph()
        assets.open("mobile_binary_graph.binarypb").use {
            val graphBytes = it.readBytes()
            graph.loadBinaryGraph(graphBytes)
        }
        // Подписка на выходной поток
        graph.addPacketCallback("out") {
            val res = PacketGetter.getFloat32Vector(it)
            val label = res.indices.maxBy { i -> res[i] } ?: -1
            this@MainActivity.runOnUiThread {
                outputTv.text = label.toString()
            }
        }
        graph.startRunningGraph()
        // Кнопка для выбора изображения из галереи
        button.setOnClickListener {
            val intent = Intent()
            intent.type = "image/*"
            intent.action = Intent.ACTION_GET_CONTENT
            startActivityForResult(Intent.createChooser(intent, "Select Picture"), PICK_IMAGE)
        }
        mpGraph = graph
    }
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == PICK_IMAGE) {
            // Получение выбранного изображения из галереи и его отрисовка
            val outputTv = findViewById<TextView>(R.id.outputTv)
            val imageView = findViewById<ImageView>(R.id.imageView)
            val uri = data?.data!!
            // Отправка изображения в граф
            val graph = mpGraph!!
            val creator = AndroidPacketCreator(graph)
            val stream = contentResolver.openInputStream(uri)
            val bitmap = BitmapFactory.decodeStream(stream)
            imageView.setImageBitmap(bitmap)
            val packet = creator.createRgbImageFrame(bitmap)
            graph.addPacketToInputStream("in", packet, timestamp)
        }
    }
    companion object {
        init {
            // Загрузка нативной mediapipe библиотеки
            System.loadLibrary("mediapipe_jni")
        }
    }
}
