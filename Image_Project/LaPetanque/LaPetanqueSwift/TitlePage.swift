//
//  TitlePage.swift
//  LaPetanque
//
//  Created by Anna Diani on 14/11/2021.
//

import SwiftUI

struct TitlePage: View {
    @State var showContentView: Bool = false
    var body: some View {
        
        if showContentView {
            ContentView()
        }
        else {
        Text("LA PETANQUE")
            .font(.title)
            .fontWeight(.bold)
            .foregroundColor(Color.white)
            .multilineTextAlignment(.center)
        
        Image("IMG_6389")
            .aspectRatio(contentMode: .fit)
            
            Button("Prendre une photo") {
                self.showContentView = true
            }
        }
    }
}

struct TitlePage_Previews: PreviewProvider {
    static var previews: some View {
        TitlePage()
    }
}
