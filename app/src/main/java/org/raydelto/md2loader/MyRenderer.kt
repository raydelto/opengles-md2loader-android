package org.raydelto.md2loader

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MyRenderer : GLSurfaceView.Renderer {
    // Native functions
    private external fun drawFrame()
    private external fun surfaceCreated()
    external fun setScreenSize(width: Int, height: Int)
    external fun setRotationAngles(x: Float, y: Float)
    external fun setScale(scale: Float)

    override fun onDrawFrame(gl: GL10?) {
        drawFrame()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        gl?.glViewport(0, 0, width, height)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        surfaceCreated()
    }

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
