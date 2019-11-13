//
//  AGEVideoContainer.swift
//  AGE
//
//  Created by CavanSu on 2019/7/23.
//  Copyright © 2019 CavanSu. All rights reserved.
//

#if os(iOS)
import UIKit
#else
import Cocoa
#endif

protocol AGEVideoContainerDataSource: NSObjectProtocol {
    func container(_ container: AGEVideoContainer, numberOfItemsIn level: Int) -> Int
    func container(_ container: AGEVideoContainer, cellForItemAt index: AGEIndex) -> AGEView
}

class AGEVideoContainer: AGEView {
    class AGLevelItem: NSObject {
        enum ViewType {
            case view(AGEScrollView)
            
            var view: AGEScrollView {
                switch self {
                case .view(let aView): return aView
                }
            }
        }
        
        var layout: AGEVideoLayout
        var viewType: ViewType
        var layoutConstraints: [NSLayoutConstraint]
        var itemsConstraints: [NSLayoutConstraint]?
        
        init(layout: AGEVideoLayout, viewType: ViewType, layoutConstraints: [NSLayoutConstraint], itemsConstraints: [NSLayoutConstraint]?) {
            self.layout = layout
            self.viewType = viewType
            self.layoutConstraints = layoutConstraints
            self.itemsConstraints = itemsConstraints
        }
    }
    
    typealias ListCountBlock = ((_ level: Int) -> Int)?
    typealias ListItemBlock = ((_ index: AGEIndex) -> AGEView)?
    typealias ListRankRowBlock = ((_ level: Int) -> AGERankRow)?
    
    private lazy var levels = [Int: AGLevelItem]()
    
    private var firstLayout = false
    
    private var listCount: ListCountBlock = nil
    private var listItem: ListItemBlock = nil
    
    private let animationTime: TimeInterval = 0.3
    
    weak var dataSource: AGEVideoContainerDataSource?
}

extension AGEVideoContainer {
    func reload(level: Int, animated: Bool = false) {
        guard let levelItem = levels[level] else {
            return
        }
        
        let view = levelItem.viewType.view
        
        guard let itemsConstraints = updateItemViewsConstrains(layoutView: view, layout: levelItem.layout) else {
            return
        }
        
        if let olds = levelItem.itemsConstraints {
            NSLayoutConstraint.deactivate(olds)
        }
        
        levelItem.itemsConstraints = itemsConstraints
        NSLayoutConstraint.activate(itemsConstraints)
        
        if animated {
            #if os(iOS)
            AGEView.animate(withDuration: animationTime) { [weak self] in
                self?.layoutIfNeeded()
            }
            #else
            if #available(OSX 10.12, *) {
                NSAnimationContext.runAnimationGroup { [weak self] (context) in
                    guard let strongSelf = self else {
                        return
                    }
                    context.duration = strongSelf.animationTime
                    context.allowsImplicitAnimation = true
                    strongSelf.layoutIfNeeded()
                }
            }
            #endif
        }
    }
    
    func setLayouts(_ layouts: [AGEVideoLayout], animated: Bool = false) {
        guard layouts.count > 0 else {
            return
        }
        
        let sorted = layouts.sorted { (item1, item2) -> Bool in
            return item1.level < item2.level
        }
        
        if !firstLayout {
            self.firstLayout = true
            self.layoutIfNeeded()
        }
        
        DispatchQueue.main.async { [weak self] in
            guard let strongSelf = self else {
                return
            }
            
            for item in sorted {
                strongSelf.updateSingalLayout(item, animated: animated)
            }
        }
    }
    
    func removeLayout(level: Int) {
        guard let levelItem = levels[level] else {
           return
        }
        let view = levelItem.viewType.view
        view.removeFromSuperview()
        levels.removeValue(forKey: level)
    }
    
    @discardableResult func listCount(_ block: ListCountBlock) -> AGEVideoContainer {
        self.listCount = block
        return self
    }
    
    @discardableResult func listItem(_ block: ListItemBlock) -> AGEVideoContainer {
        self.listItem = block
        return self
    }
}

private extension AGEVideoContainer {
    func updateSingalLayout(_ layout: AGEVideoLayout, animated: Bool = false) {
        if let levelItem = levels[layout.level] {
            let oldLayout = levelItem.layout
            let result = oldLayout.isEqual(right: layout)
            
            switch result {
            case .yes: break
            case .no(let reasons):
                levelItem.layout = layout
                levels[layout.level] = levelItem
                
                let view = levelItem.viewType.view
                
                var newConstraints = [NSLayoutConstraint]()
                
                if reasons.containsOneOf([.minimumInteritemSpacing,
                                          .minimumLineSpacing,
                                          .itemSize,
                                          .layoutType]) {
                    view.contentOffset = CGPoint.zero
                    
                    if let olds = levelItem.itemsConstraints {
                        NSLayoutConstraint.deactivate(olds)
                    }
                    
                    if let itemsConstraints = updateItemViewsConstrains(layoutView: view, layout: layout) {
                        levelItem.itemsConstraints = itemsConstraints
                        newConstraints.append(contentsOf: itemsConstraints)
                    }
                }
                
                if reasons.containsOneOf([.startPoint,
                                          .size]) {
                    let olds = levelItem.layoutConstraints
                    NSLayoutConstraint.deactivate(olds)
                    
                    let layoutConstraints = updateLayoutViewConstraints(view, layout: layout)
                    levelItem.layoutConstraints = layoutConstraints
                    
                    newConstraints.append(contentsOf: layoutConstraints)
                }
                
                NSLayoutConstraint.activate(newConstraints)
                
                if animated {
                    #if os(iOS)
                    AGEView.animate(withDuration: animationTime) { [weak self] in
                        self?.layoutIfNeeded()
                    }
                    #else
                    if #available(OSX 10.12, *) {
                        NSAnimationContext.runAnimationGroup { [weak self] (context) in
                            guard let strongSelf = self else {
                                return
                            }
                            
                            context.duration = strongSelf.animationTime
                            context.allowsImplicitAnimation = true
                            strongSelf.layoutIfNeeded()
                        }
                    }
                    #endif
                }
                
                levels[layout.level] = levelItem
            }
        } else {
            createSingalLayout(layout)
        }
    }
    
    func createSingalLayout(_ layout: AGEVideoLayout) {
        let number = numberOfList(layout.level)
        guard number > 0 else {
            return
        }
        
        // layoutView
        let layoutView = AGEScrollView()
        
        self.addSubview(layoutView)
        let layoutConstraints = updateLayoutViewConstraints(layoutView, layout: layout)
        NSLayoutConstraint.activate(layoutConstraints)
        layoutView.layoutIfNeeded()

        DispatchQueue.main.async { [weak self] in
            guard let strongSelf = self else {
                return
            }

            // itemViews
            let itemsConstraints = strongSelf.updateItemViewsConstrains(layoutView: layoutView, layout: layout)

            strongSelf.levels[layout.level] = AGLevelItem(layout: layout,
                                                    viewType: .view(layoutView),
                                                    layoutConstraints: layoutConstraints,
                                                    itemsConstraints: itemsConstraints)

            if let constraints = itemsConstraints {
                NSLayoutConstraint.activate(constraints)
            }
        }
    }
}

private extension AGEVideoContainer {
    func updateLayoutViewConstraints(_ view: AGEView, layout: AGEVideoLayout) -> [NSLayoutConstraint] {
        view.translatesAutoresizingMaskIntoConstraints = false
        let startPoint = layout.startPoint
        let widthScale = CGFloat((layout.size.width > 1 ? 1 : layout.size.width))
        let heightScale = CGFloat((layout.size.height > 1 ? 1 : layout.size.height))
        let layoutConstraints = addConstraints(for: view,
                                               on: self,
                                               startPoint: startPoint,
                                               widthScale: widthScale,
                                               heightScale: heightScale)
        return layoutConstraints
    }
    
    func updateItemViewsConstrains(layoutView: AGEScrollView, layout: AGEVideoLayout) -> [NSLayoutConstraint]? {
        let number = numberOfList(layout.level)
        guard number > 0 else {
            return nil
        }
        
        var itemViews = [AGEView]()
        
        for index in 0..<number {
            if let view = viewOfList(level: layout.level, item: index) {
                itemViews.append(view)
            }
        }
        
        var itemsConstraints = [NSLayoutConstraint]()
        
        var rankCount: Int
        var rowCount: Int
        var widthScale: CGFloat
        var heightScale: CGFloat
        
        #if os(iOS)
        layoutView.isScrollEnabled = layout.type.isScroll
//        layoutView.isPagingEnabled = layout.type.isScroll
        #endif
        
        var interitemSpacing: CGFloat = 0
        var lineSpacing: CGFloat = 0
        
        switch layout.type {
        case .view:
            if layout.itemSize.width > 0.5 {
                rankCount = 1
            } else  {
                rankCount = Int(1.0 / layout.itemSize.width)
            }
            
            if layout.itemSize.height > 0.5 {
                rowCount = 1
            } else  {
                rowCount = Int(1.0 / layout.itemSize.height)
            }
            
            let iSpacing = layout.interitemSpacing / layoutView.bounds.width
            let interitemSpacingScale = (iSpacing >= 1 ? 0 : iSpacing)
            
            let lSpacing = layout.lineSpacing / layoutView.bounds.height
            let lineSpacingScale = (lSpacing >= 1 ? 0 : lSpacing)
            
            widthScale = (1.0 - (interitemSpacingScale * CGFloat(rankCount - 1))) / CGFloat(rankCount)
            heightScale = (1.0 - (lineSpacingScale * CGFloat(rowCount - 1))) / CGFloat(rowCount)
            
            interitemSpacing = layout.interitemSpacing
            lineSpacing = layout.lineSpacing
            
            #if os(macOS)
            if let view = layoutView.documentView {
                let constrains = addConstraints(for: view, on: layoutView, startPoint: CGPoint.zero, widthScale: 1, heightScale: 1)
                itemsConstraints.append(contentsOf: constrains)
            }
            #endif
        #if os(iOS)
        case .scroll(let direction):
            switch direction {
            case .horizontal:
                let contentWidth = CGFloat(number) * self.bounds.width * layout.size.width * layout.itemSize.width + CGFloat(number - 1) * layout.interitemSpacing
                #if os(iOS)
                layoutView.contentSize = AGESize(width: contentWidth, height: 0)
                #else
                layoutView.documentView?.frame = NSMakeRect(0, 0, contentWidth, 0)
                #endif
                rowCount = 1
                rankCount = number
                heightScale = 1.0
                widthScale = layout.itemSize.width
                interitemSpacing = layout.interitemSpacing
            case .vertical:
                let contentHeight = CGFloat(number) * self.bounds.height * layout.size.height * layout.itemSize.height + CGFloat(number - 1) * layout.lineSpacing
                #if os(iOS)
                layoutView.contentSize = AGESize(width: 0, height: contentHeight)
                #else
                layoutView.documentView?.frame = NSMakeRect(0, 0, 0, contentHeight)
                #endif
                rankCount = 1
                rowCount = number
                widthScale = 1.0
                heightScale = layout.itemSize.height
                lineSpacing = layout.lineSpacing
            }
        #endif
        }
        
        var superView: AGEView
        #if os(iOS)
        superView = layoutView
        #else
        superView = layoutView.documentView!
        #endif
        
        let rankRow = AGERankRow(ranks: rankCount, rows: rowCount)
        let constraints = addConstraints(for: itemViews,
                                         on: superView,
                                         rankRow: rankRow,
                                         widthScale: widthScale,
                                         heightScale: heightScale,
                                         interitemSpacing: interitemSpacing,
                                         lineSpacing: lineSpacing)
        itemsConstraints.append(contentsOf: constraints)
        
        return itemsConstraints
    }
}

private extension AGEVideoContainer {
    func addConstraints(for subView: AGEView, on superView: AGEView, startPoint: CGPoint, widthScale: CGFloat, heightScale: CGFloat) -> [NSLayoutConstraint] {
        subView.translatesAutoresizingMaskIntoConstraints = false
        var constraints = [NSLayoutConstraint]()
        
        let left = NSLayoutConstraint(item: subView,
                                      attribute: .left,
                                      relatedBy: .equal,
                                      toItem: superView,
                                      attribute: .left,
                                      multiplier: 1,
                                      constant: startPoint.x)
        constraints.append(left)
        
        let top = NSLayoutConstraint(item: subView,
                                     attribute: .top,
                                     relatedBy: .equal,
                                     toItem: superView,
                                     attribute: .top,
                                     multiplier: 1,
                                     constant: startPoint.y)
        constraints.append(top)
        
        let whContraints = addConstraints(for: subView,
                                          on: superView,
                                          widthScale: widthScale,
                                          heightScale: heightScale)
        constraints.append(contentsOf: whContraints)
        
        return constraints
    }
    
    func addConstraints(for subView: AGEView, on superView: AGEView, widthScale: CGFloat, heightScale: CGFloat) -> [NSLayoutConstraint] {
        subView.translatesAutoresizingMaskIntoConstraints = false
        var constraints = [NSLayoutConstraint]()
        
        let width = NSLayoutConstraint(item: subView,
                                       attribute: .width,
                                       relatedBy: .equal,
                                       toItem: superView,
                                       attribute: .width,
                                       multiplier: widthScale,
                                       constant: 0)
        constraints.append(width)
        
        let height = NSLayoutConstraint(item: subView,
                                        attribute: .height,
                                        relatedBy: .equal,
                                        toItem: superView,
                                        attribute: .height,
                                        multiplier: heightScale,
                                        constant: 0)
        constraints.append(height)
        
        return constraints
    }
    
    func addConstraints(for itemViews: [AGEView], on superView: AGEView, rankRow: AGERankRow, widthScale: CGFloat, heightScale: CGFloat, interitemSpacing: CGFloat, lineSpacing: CGFloat) -> [NSLayoutConstraint] {
        var constraints = [NSLayoutConstraint]()
        var lastView: AGEView?
     
        for row in 0..<rankRow.rows where row < itemViews.count {
            var index: Int = 0
            guard index < itemViews.count else {
                break
            }
            
            for rank in 0..<rankRow.ranks {
                index = row * rankRow.ranks + rank
                
                guard index < itemViews.count else {
                    break
                }
                
                let view = itemViews[index]
                view.removeFromSuperview()
                view.translatesAutoresizingMaskIntoConstraints = false
                superView.addSubview(view)
                
                // first rank view of per rows
                if rank == 0 {
                    let left = NSLayoutConstraint(item: view,
                                                  attribute: .left,
                                                  relatedBy: .equal,
                                                  toItem: superView,
                                                  attribute: .left,
                                                  multiplier: 1,
                                                  constant: 0)
                    constraints.append(left)
                    
                    if row == 0 {
                        let top = NSLayoutConstraint(item: view,
                                                     attribute: .top,
                                                     relatedBy: .equal,
                                                     toItem: superView,
                                                     attribute: .top,
                                                     multiplier: 1,
                                                     constant: 0)
                        constraints.append(top)
                    } else {
                        let lastRowIndex = index - rankRow.ranks
                        let last = lastRowIndex < 0 ? 0 : lastRowIndex
                        let lastRowView = itemViews[last]
                        let top = NSLayoutConstraint(item: lastRowView,
                                                     attribute: .bottom,
                                                     relatedBy: .equal,
                                                     toItem: view,
                                                     attribute: .top,
                                                     multiplier: 1,
                                                     constant: -lineSpacing)
                        constraints.append(top)
                    }
                } else if let lastView = lastView {
                    let left = NSLayoutConstraint(item: view,
                                                  attribute: .left,
                                                  relatedBy: .equal,
                                                  toItem: lastView,
                                                  attribute: .right,
                                                  multiplier: 1,
                                                  constant: interitemSpacing)
                    constraints.append(left)
                    
                    let top = NSLayoutConstraint(item: view,
                                                 attribute: .top,
                                                 relatedBy: .equal,
                                                 toItem: lastView,
                                                 attribute: .top,
                                                 multiplier: 1,
                                                 constant: 0)
                    constraints.append(top)
                }
                
                let whConstraints = addConstraints(for: view,
                                                   on: superView,
                                                   widthScale: widthScale,
                                                   heightScale: heightScale)
                constraints.append(contentsOf: whConstraints)
                
                lastView = view
            }
        }
        return constraints
    }
}

private extension AGEVideoContainer {
    func numberOfList(_ level: Int) -> Int {
        var count: Int = 0
        if let listCount = self.listCount {
            count = listCount(level)
        } else if let number = dataSource?.container(self, numberOfItemsIn: level) {
            count = number
        }
        return count
    }
    
    func viewOfList(level: Int, item: Int) -> AGEView? {
        var itemView: AGEView?
        if let listItem = self.listItem {
            let index = AGEIndex(level: level, item: item)
            itemView = listItem(index)
        } else if let view = dataSource?.container(self, cellForItemAt: AGEIndex(level: level, item: item)) {
            itemView = view
        }
        return itemView
    }
}
