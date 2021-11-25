//
//  Motion.swift
//  LaPetanque
//
//  Created by Raphael Braud-Mussi on 22/11/2021.
//

import Foundation


import SwiftUI
import CoreMotion



import SwiftUI

struct ParallaxMotionTestView: View {
    
    @ObservedObject var manager = MotionManager()
    
    var body: some View {
        Rectangle()
            .strokeBorder(Color.red,lineWidth: 4)
            .frame(width: 75, height: 75, alignment: .center)
            .modifier(ParallaxMotionModifier(manager: manager, magnitude: 100))
            .overlay(
                Rectangle()
                
                    .frame(width: 100, height: 100, alignment: .center)
                    .modifier(ParallaxMotionModifier(manager: manager, magnitude: 50))
                ,alignment: .center)
    }
}

struct ParallaxMotionModifier: ViewModifier {
    
    @ObservedObject var manager: MotionManager
    var magnitude: Double
    
    func body(content: Content) -> some View {
        content
            .offset(x: CGFloat(manager.roll * magnitude), y: CGFloat(manager.pitch * magnitude))
    }
}

class MotionManager: ObservableObject {
    
    @Published var pitch: Double = 0.0
    @Published var roll: Double = 0.0
    
    private var manager: CMMotionManager
    
    init() {
        self.manager = CMMotionManager()
        self.manager.deviceMotionUpdateInterval = 1/60
        self.manager.startDeviceMotionUpdates(to: .main) { (motionData, error) in
            guard error == nil else {
                print(error!)
                return
            }
            
            if let motionData = motionData {
                self.pitch = motionData.attitude.pitch
                self.roll = motionData.attitude.roll
            }
        }
        
    }
}
