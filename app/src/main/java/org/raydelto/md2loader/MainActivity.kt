package org.raydelto.md2loader

import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.Log
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import android.view.VelocityTracker
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import java.io.File
import java.io.RandomAccessFile
import kotlin.math.max
import kotlin.math.min


class MainActivity : AppCompatActivity() {

    private lateinit var glSurfaceView: GLSurfaceView

    private var assetsDirectory: String? = null

    private val TAG = "MainActivity"

    private var velocityTracker: VelocityTracker? = null

    private var renderer: MyRenderer? = null

    private var scaleFactor = 1f

    private var scaleDetector: ScaleGestureDetector? = null

    private val scaleListener = object : ScaleGestureDetector.SimpleOnScaleGestureListener() {

        override fun onScale(detector: ScaleGestureDetector): Boolean {
            scaleFactor *= detector.scaleFactor

            // Don't let the object get too small or too large.
            scaleFactor = max(0.1f, min(scaleFactor, 5.0f))
            Log.e(TAG, "scalefactor = $scaleFactor")
            renderer?.setScale(scaleFactor)

            return true
        }
    }


    override fun onTouchEvent(event: MotionEvent): Boolean {
        if (event.pointerCount > 1) {
            scaleDetector?.onTouchEvent(event)
            return true
        }

        when (event.actionMasked) {
            MotionEvent.ACTION_DOWN -> {
                // Reset the velocity tracker back to its initial state.
                velocityTracker?.clear()
                // If necessary retrieve a new VelocityTracker object to watch the
                // velocity of a motion.
                velocityTracker = velocityTracker ?: VelocityTracker.obtain()
                // Add a user's movement to the tracker.
                velocityTracker?.addMovement(event)
                renderer?.setRotationAngles(0.0f, 0.0f)
            }
            MotionEvent.ACTION_MOVE -> {
                velocityTracker?.apply {
                    val pointerId: Int = event.getPointerId(event.actionIndex)
                    addMovement(event)
                    // When you want to determine the velocity, call
                    // computeCurrentVelocity(). Then call getXVelocity()
                    // and getYVelocity() to retrieve the velocity for each pointer ID.
                    computeCurrentVelocity(2)
                    renderer?.setRotationAngles(getXVelocity(pointerId), getYVelocity(pointerId))
                }
            }
            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                // Return a VelocityTracker object back to be re-used by others.
                velocityTracker?.recycle()
                velocityTracker = null
                renderer?.setRotationAngles(0.0f, 0.0f)
            }
        }
        return true
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        scaleDetector = ScaleGestureDetector(applicationContext, scaleListener)
        setContentView(R.layout.activity_drawing)
        glSurfaceView = findViewById<GLSurfaceView>(R.id.glSurfaceView)
        glSurfaceView.setEGLContextClientVersion(2)

        assetsDirectory = applicationContext.filesDir.path + "/"
        val assets: List<String> = listOf("female.md2", "female.tga","grunt.md2", "grunt.tga",  "basic.vert", "basic.frag")

        for (asset in assets) {
            extractAsset(asset)
        }

        val metrics = resources.displayMetrics
        val height = metrics.heightPixels
        val width = metrics.widthPixels
        renderer = MyRenderer()
        renderer?.setScreenSize(width, height)
        glSurfaceView.setRenderer(renderer)
    }

    private fun extractAsset(assetName: String) {
        val file = File(assetsDirectory + assetName)
        if (file.exists()) {
            Log.d(TAG, "$assetName already exists. No extraction needed.\n")
        } else {
            Log.d(TAG, "$assetName doesn't exist. Extraction needed. \n")
            try {
                val randomAccessFile = RandomAccessFile(assetsDirectory + assetName, "rw")
                val assetManager: AssetManager = applicationContext.resources.assets
                val inputStream = assetManager.open(assetName)
                val buffer = ByteArray(1024)
                var count = inputStream.read(buffer, 0, 1024)
                while (count > 0) {
                    randomAccessFile.write(buffer, 0, count)
                    count = inputStream.read(buffer, 0, 1024)
                }
                randomAccessFile.close()
                inputStream.close()
            } catch (e: Exception) {
                Log.e(TAG, "Failure in extractAssets():  $e  \n $assetsDirectory  $assetName")
            }
            if (file.exists()) {
                Log.d(TAG, "File extracted successfully")
            }
        }
    }

    override fun onPause() {
        super.onPause()
        glSurfaceView.onPause()
    }

    override fun onResume() {
        super.onResume()
        glSurfaceView.onResume()
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (hasFocus) hideSystemUI()
    }

    private fun hideSystemUI() {
        // Enables regular immersive mode.
        // For "lean back" mode, remove SYSTEM_UI_FLAG_IMMERSIVE.
        // Or for "sticky immersive," replace it with SYSTEM_UI_FLAG_IMMERSIVE_STICKY
        window.decorView.systemUiVisibility = (View.SYSTEM_UI_FLAG_IMMERSIVE
                // Set the content to appear under the system bars so that the
                // content doesn't resize when the system bars hide and show.
                or View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                or View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                // Hide the nav bar and status bar
                or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_FULLSCREEN)
    }
}
