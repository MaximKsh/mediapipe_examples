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

        // Initialize asset manager so that MediaPipe native libraries can access the app assets, e.g.,
        // binary graphs.
        AndroidAssetUtil.initializeNativeAssetManager(this)

        val graph = Graph()
        assets.open("mobile_binary_graph.binarypb").use {
            val graphBytes = it.readBytes()
            graph.loadBinaryGraph(graphBytes)
        }

        graph.addPacketCallback("out") {
            val res = PacketGetter.getFloat32Vector(it)
            val label = res.indices.maxBy { i -> res[i] } ?: -1

            this@MainActivity.runOnUiThread {
                outputTv.text = label.toString()
            }
        }
        graph.startRunningGraph()

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
            val outputTv = findViewById<TextView>(R.id.outputTv)
            val imageView = findViewById<ImageView>(R.id.imageView)
            val uri = data?.data!!

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
            System.loadLibrary("mediapipe_jni")
        }
    }

}
