//
//  ContentView.swift
//  LaPetanque
//
//  Created by Raphael Braud-Mussi on 08/11/2021.
//

import SwiftUI

struct ContentView: View {
    @State private var showPicker : Bool = false
    @State private var image : Image? = nil
    var body: some View {
        
        image?.resizable()
            .scaledToFit()
        VStack{
            Button("Appareil photo") {
                self.showPicker = true
            }
            .padding(20)
            .background(Color.blue)
            .foregroundColor(Color.white)
            .cornerRadius(20)
            
        }.sheet(isPresented: self.$showPicker){
            PhotoCapture(showPicker: self.$showPicker, image: self.$image)
        }
        
        
        
            
            
        
        
        
        
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
