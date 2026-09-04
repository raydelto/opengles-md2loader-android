//
//  MyRenderer.swift
//  MD2Loader
//

import Foundation
import OpenGLES.ES2

final class MyRenderer {
    // Native functions bridged to C++ Renderer
    func surfaceCreated() {
        MD2RendererBridge.surfaceCreated()
    }

    func drawFrame() {
        MD2RendererBridge.drawFrame()
    }

    func setScreenSize(width: Int32, height: Int32) {
        MD2RendererBridge.setScreenSizeWithWidth(width, height: height)
    }

    func setRotationAngles(x: Float, y: Float) {
        MD2RendererBridge.setRotationAnglesWithX(x, y: y)
    }

    func setScale(_ scale: Float) {
        MD2RendererBridge.setScale(scale)
    }

    func setAssetPath(_ path: String) {
        MD2RendererBridge.setAssetPath(path)
    }

    // Lifecycle events corresponding to GLSurfaceView.Renderer
    func onSurfaceCreated() {
        surfaceCreated()
    }

    func onSurfaceChanged(width: Int32, height: Int32) {
        glViewport(0, 0, width, height)
    }

    func onDrawFrame() {
        drawFrame()
    }
}
