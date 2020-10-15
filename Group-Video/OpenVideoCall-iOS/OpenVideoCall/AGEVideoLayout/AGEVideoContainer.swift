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

protocol AGEVideoContainerDelegate: NSObjectProtocol {
    func container(_ container: AGEVideoContainer, didSelected subView: AGEView, index: AGEIndex)
}

extension AGEVideoContainerDelegate {
    func container(_ container: AGEVideoContainer, didSelected subView: AGEView, index: AGEIndex) {}
}

class AGEVideoContainer: AGEView {
    
    class AGELog: NSObject {
        static var currentNeedLog: AGELogType? = nil
        
        static var separator: String = "----------------"
        
        struct AGELogType: OptionSet {
            var rawValue: Int
            
            static func all() -> Int {
                return AGELogType.eventsTrain.rawValue
            }
            
            static let eventsTrain = AGELogType(rawValue: 1)
            static let updateLayoutConstraints = AGELogType(rawValue: 1 << 1)
            
            var tag: String {
                if self == AGELogType.eventsTrain {
                    return "Events train"
                } else if self == AGELogType.updateLayoutConstraints {
                    return "Update Constraints"
                } else {
                    return "All"
                }
            }
        }
        
        static func log(_ content: String, type: AGELogType) {
            guard let needLog = currentNeedLog else {
                return
            }
            
            func log(content: String, tag: String) {
                NSLog("AGELog - \(tag): \(content)")
            }
            
            if needLog.rawValue == AGELogType.all() {
                log(content: content, tag: needLog.tag)
                return
            }
            
            if needLog.contains(type) {
                log(content: content, tag: type.tag)
            }
        }
    }
    
    class AGELevelItem: NSObject {
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
        
        init(layout: AGEVideoLayout,
             viewType: ViewType,
             layoutConstraints: [NSLayoutConstraint],
             itemsConstraints: [NSLayoutConstraint]?) {
            
            self.layout = layout
            self.viewType = viewType
            self.layoutConstraints = layoutConstraints
            self.itemsConstraints = itemsConstraints
        }
    }
    
    typealias ListCountBlock = ((_ level: Int) -> Int)?
    typealias ListItemBlock = ((_ index: AGEIndex) -> AGEView)?
    typealias ListRankRowBlock = ((_ level: Int) -> AGERankRow)?
    
    private lazy var levels = [Int: AGELevelItem]()
    private var eventsObserver = AGEEventsObserver()
    
    private var firstLayout = false
    
    private var listCount: ListCountBlock = nil
    private var listItem: ListItemBlock = nil
    
    private let animationTime: TimeInterval = 0.3
    
    weak var dataSource: AGEVideoContainerDataSource?
    weak var delegate: AGEVideoContainerDelegate?
    
    #if os(macOS)
    override func viewDidMoveToWindow() {
        super.viewDidMoveToWindow()
        addEventsObserver()
    }
    #else
    override init(frame frameRect: CGRect) {
        super.init(frame: frameRect)
        addTapGesture()
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        addTapGesture()
    }
    
    init() {
        super.init(frame: CGRect(x: 0, y: 0, width: 0, height: 0))
        addTapGesture()
    }
    #endif
}

extension AGEVideoContainer {
    func reload(level: Int, animated: Bool = false) {
        guard let levelItem = levels[level] else {
            return
        }
        
        let layoutView = levelItem.viewType.view
        layoutView.removeAllItemViews()
            
        let itemsConstraints = updateItemViewsConstrains(layoutView: layoutView,
                                                         layout: levelItem.layout)
        
        if let olds = levelItem.itemsConstraints {
            NSLayoutConstraint.deactivate(olds)
        }
        
        if let news = itemsConstraints {
            levelItem.itemsConstraints = itemsConstraints
            NSLayoutConstraint.activate(news)
        }
       
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
    
    func removeAllLayouts() {
        let allLevels = levels.keys
        
        for level in allLevels {
            removeLayout(level: level)
        }
    }
    
    func removeLayout(level: Int) {
        guard let levelItem = levels[level] else {
           return
        }
        NSLayoutConstraint.deactivate(levelItem.layoutConstraints)
        if let itemsConstraints = levelItem.itemsConstraints {
            NSLayoutConstraint.deactivate(itemsConstraints)
        }
        let layoutView = levelItem.viewType.view
        layoutView.removeAllItemViews()
        layoutView.removeFromSuperview()
        levels.removeValue(forKey: level)
    }
    
    func setup(logLevel: AGELog.AGELogType) {
        AGEVideoContainer.AGELog.currentNeedLog = logLevel
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
                AGELog.log("reasons: \(reasons.description)", type: .updateLayoutConstraints)
                
                levelItem.layout = layout
                levels[layout.level] = levelItem
                
                let layoutView = levelItem.viewType.view
                
                var newConstraints = [NSLayoutConstraint]()
                
                // Reset ItemViews constraints
                if reasons.containsOneOf([.interitemSpacing,
                                          .lineSpacing,
                                          .itemSize,
                                          .scrollType]) {
                    
                    layoutView.removeAllItemViews()
                    layoutView.contentOffset = CGPoint.zero
                                        
                    if let olds = levelItem.itemsConstraints {
                        NSLayoutConstraint.deactivate(olds)
                        AGELog.log("old itemsConstraints count: \(olds.count)", type: .updateLayoutConstraints)
                    }
                    
                    if let itemsConstraints = updateItemViewsConstrains(layoutView: layoutView, layout: layout) {
                        AGELog.log("new itemsConstraints count: \(itemsConstraints.count)", type: .updateLayoutConstraints)
                        levelItem.itemsConstraints = itemsConstraints
                        newConstraints.append(contentsOf: itemsConstraints)
                    }
                }
                
                // Reset LayoutView constraints
                if reasons.containsOneOf([.startPoint,
                                          .size]) {
                    
                    let olds = levelItem.layoutConstraints
                    NSLayoutConstraint.deactivate(olds)
                    
                    let layoutConstraints = updateLayoutViewConstraints(layoutView, layout: layout)
                    levelItem.layoutConstraints = layoutConstraints
                    
                    newConstraints.append(contentsOf: layoutConstraints)
                }
                
                NSLayoutConstraint.activate(newConstraints)
                
                // Animation
                layoutAnimation(animated)
                
                levels[layout.level] = levelItem
            }
        } else {
            createSingalLayout(layout, animated: animated)
        }
    }
    
    func createSingalLayout(_ layout: AGEVideoLayout, animated: Bool = false) {
        // layoutView
        let layoutView = AGEScrollView()
        let layoutConstraints = updateLayoutViewConstraints(layoutView, layout: layout)
        NSLayoutConstraint.activate(layoutConstraints)
        layoutView.layoutIfNeeded()

        DispatchQueue.main.async { [weak self] in
            guard let strongSelf = self else {
                return
            }

            // itemViews
            let itemsConstraints = strongSelf.updateItemViewsConstrains(layoutView: layoutView, layout: layout)

            strongSelf.levels[layout.level] = AGELevelItem(layout: layout,
                                                           viewType: .view(layoutView),
                                                           layoutConstraints: layoutConstraints,
                                                           itemsConstraints: itemsConstraints)

            if let constraints = itemsConstraints {
                NSLayoutConstraint.activate(constraints)
                strongSelf.layoutAnimation(animated)
            }
        }
    }
}

private extension AGEVideoContainer {
    func layoutAnimation(_ animated: Bool) {
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
    
    func updateLayoutViewConstraints(_ view: AGEView, layout: AGEVideoLayout) -> [NSLayoutConstraint] {
        let startPoint = layout.startPoint
        let layoutConstraints = AGEVideoConstraints.add(for: view,
                                                        on: self,
                                                        startPoint: startPoint,
                                                        whSize: layout.size)
                
        return layoutConstraints
    }
    
    func updateItemViewsConstrains(layoutView: AGEScrollView, layout: AGEVideoLayout) -> [NSLayoutConstraint]? {
        guard let itemViews = viewsOfList(level: layout.level) else {
            return nil
        }
        
        let number = itemViews.count
        let itemSize = layout.itemSize
        var itemsConstraints = [NSLayoutConstraint]()
        
        var rankCount: Int
        var rowCount: Int
        var width: CGFloat
        var height: CGFloat
        
        #if os(iOS)
        layoutView.isScrollEnabled = layout.scrollType.isScroll
//        layoutView.isPagingEnabled = layout.type.isScroll
        #endif
        
        var interitemSpacing: CGFloat = 0
        var lineSpacing: CGFloat = 0
        
        switch layout.scrollType {
        case .static:
            
            switch itemSize {
            case .scale:
                if itemSize.width > 0.5 {
                    rankCount = 1
                } else  {
                    rankCount = Int(1.0 / itemSize.width)
                }
                
                if itemSize.height > 0.5 {
                    rowCount = 1
                } else  {
                    rowCount = Int(1.0 / itemSize.height)
                }
                
                let iSpacing = layout.interitemSpacing / layoutView.bounds.width
                let interitemSpacingScale = (iSpacing >= 1 ? 0 : iSpacing)
                
                let lSpacing = layout.lineSpacing / layoutView.bounds.height
                let lineSpacingScale = (lSpacing >= 1 ? 0 : lSpacing)
                
                width = (1.0 - (interitemSpacingScale * CGFloat(rankCount - 1))) / CGFloat(rankCount)
                height = (1.0 - (lineSpacingScale * CGFloat(rowCount - 1))) / CGFloat(rowCount)
            case .constant:
                if itemSize.width > layoutView.bounds.width * 0.5 {
                    rankCount = 1
                } else {
                    rankCount = Int(layoutView.bounds.width / itemSize.width)
                }
                
                if itemSize.height > layoutView.bounds.height * 0.5 {
                    rowCount = 1
                } else  {
                    rowCount = Int(layoutView.bounds.height / itemSize.height)
                }
                
                width = itemSize.width
                height = itemSize.height
            }
            
            interitemSpacing = layout.interitemSpacing
            lineSpacing = layout.lineSpacing
            
            #if os(macOS)
            if let view = layoutView.documentView {
                let constrains = AGEVideoConstraints.add(for: view,
                                                         on: layoutView,
                                                         startPoint: CGPoint.zero,
                                                         whSize: .scale(CGSize(width: 1.0, height: 1.0)),
                                                         needAddSubView: false)
                itemsConstraints.append(contentsOf: constrains)
            }
            #endif
        case .scroll(let direction):
            switch direction {
            case .horizontal:
               
                switch layout.itemSize {
                case .scale:
                    let contentWidth = CGFloat(number) * self.bounds.width * layout.size.width * layout.itemSize.width + CGFloat(number - 1) * layout.interitemSpacing
                    #if os(iOS)
                    layoutView.contentSize = AGESize(width: contentWidth, height: 0)
                    #else
                    let view = AGEView(frame: NSRect(x: 0, y: 0, width: contentWidth, height: layoutView.bounds.height))
                    layoutView.documentView = view
                    #endif
                    
                    height = 1.0
                    #if os(iOS)
                    width = layout.itemSize.width
                    #else
                    width = (layoutView.bounds.width * layout.itemSize.width) / contentWidth
                    #endif
                case .constant:
                    let contentWidth = CGFloat(number) * layout.itemSize.width + CGFloat(number - 1) * layout.interitemSpacing
                    #if os(iOS)
                    layoutView.contentSize = AGESize(width: contentWidth, height: 0)
                    #else
                    let viewWidth = contentWidth < layoutView.bounds.width ? layoutView.bounds.width : contentWidth
                    let view = AGEView(frame: NSRect(x: 0, y: 0, width: viewWidth, height: layoutView.bounds.height))
                    layoutView.documentView = view
                    #endif
                    
                    height = layoutView.bounds.height
                    #if os(iOS)
                    width = layout.itemSize.width
                    #else
                    width = layout.itemSize.width
                    #endif
                }
                
                rowCount = 1
                rankCount = number
                interitemSpacing = layout.interitemSpacing
            case .vertical:
                
                switch layout.itemSize {
                case .scale:
                    let contentHeight = CGFloat(number) * self.bounds.height * layout.size.height * layout.itemSize.height + CGFloat(number - 1) * layout.lineSpacing
                    #if os(iOS)
                    layoutView.contentSize = AGESize(width: 0, height: contentHeight)
                    #else
                    let view = AGEView(frame: NSRect(x: 0, y: 0, width: layoutView.bounds.width, height: contentHeight))
                    layoutView.documentView = view
                    layoutView.scrollToTop()
                    #endif
                    rankCount = 1
                    rowCount = number
                    width = 1.0
                    #if os(iOS)
                    height = layout.itemSize.height
                    #else
                    height = (layoutView.bounds.height * layout.itemSize.height) / contentHeight
                    #endif
                case .constant:
                    let contentHeight = CGFloat(number) * layout.itemSize.height + CGFloat(number - 1) * layout.lineSpacing
                    #if os(iOS)
                    layoutView.contentSize = AGESize(width: 0, height: contentHeight)
                    #else
                    let viewHeight = contentHeight < layoutView.bounds.height ? layoutView.bounds.height : contentHeight
                    let view = AGEView(frame: NSRect(x: 0, y: 0, width: layoutView.bounds.width, height: viewHeight))
                    
                    layoutView.documentView = view
                    layoutView.scrollToTop()
                    #endif
                    rankCount = 1
                    rowCount = number
                    width = layoutView.bounds.width
                    #if os(iOS)
                    height = layout.itemSize.height
                    #else
                    height = layout.itemSize.height
                    #endif
                }
                
                lineSpacing = layout.lineSpacing
            }
        }
        
        var superView: AGEView
        #if os(iOS)
        superView = layoutView
        #else
        superView = layoutView.documentView!
        superView.wantsLayer = true
        superView.layer?.backgroundColor = NSColor.clear.cgColor
        #endif
        
        var whSize: AGEVideoLayout.ConstraintsType
        switch layout.itemSize {
        case .scale:
            whSize = .scale(CGSize(width: width, height: height))
        case .constant:
            whSize = .constant(CGSize(width: width, height: height))
        }
        
        let rankRow = AGERankRow(ranks: rankCount, rows: rowCount)
        let constraints = AGEVideoConstraints.add(for: itemViews,
                                                  on: superView,
                                                  rankRow: rankRow,
                                                  whSize: whSize,
                                                  interitemSpacing: interitemSpacing,
                                                  lineSpacing: lineSpacing)
        itemsConstraints.append(contentsOf: constraints)
        
        return itemsConstraints
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
    
    func viewsOfList(level: Int) -> [AGEView]? {
        let number = numberOfList(level)
        guard number > 0 else {
            return nil
        }
        
        var itemViews = [AGEView]()
        
        for index in 0..<number {
            if let view = viewOfList(level: level, item: index) {
                itemViews.append(view)
            }
        }
        
        return itemViews
    }
}

// MARK: - Events Train
private extension AGEVideoContainer {
    #if os(macOS)
    func addEventsObserver() {
        eventsObserver.delegate = self
        eventsObserver.dataSource = self
        eventsObserver.listenCurrentWindow(inputEvents: .leftMouseUp)
    }
    
    #else
    func addTapGesture() {
        let recognizer = UITapGestureRecognizer(target: self, action: nil)
        recognizer.numberOfTapsRequired = 1
        recognizer.delegate = self
        self.addGestureRecognizer(recognizer)
    }
    #endif
}

#if os(iOS)
extension AGEVideoContainer: UIGestureRecognizerDelegate {
    override func gestureRecognizerShouldBegin(_ gestureRecognizer: UIGestureRecognizer) -> Bool {
        let position = gestureRecognizer.location(in: self)
       
        findSelectedViewCommonSteps(with: AGEEvent(),
                                    scrollNeedPosition: position)
        
        return true
    }
}
#endif

#if os(macOS)
extension AGEVideoContainer: AGEEventsObserverDelegate {
    func observer(_ observer: AGEEventsObserver, didTriggerInputEvent event: NSEvent) {
        findSelectedViewOnMacSteps(with: event)
    }
}

extension AGEVideoContainer: AGEEventsObserverDataSource {
    func observerNeedWindowNumber(_ observer: AGEEventsObserver) -> Int {
        guard let windowNumber = self.window?.windowNumber else {
            return -1
        }
        
        return windowNumber
    }
}
#endif

private extension AGEVideoContainer {
    #if os(macOS)
    typealias AGEEvent = NSEvent
    #else
    typealias AGEEvent = UIEvent
    #endif
    
    struct PositionCheckResult {
        var isContain: Bool
        var convertedPosition: CGPoint
    }
    
    #if os(macOS)
    func findSelectedViewOnMacSteps(with event: AGEEvent) {
        guard let windowContentView = self.window?.contentView else {
            return
        }
        
        // Step 1, check AGEContainer contains this position
        let containerResult = check(superView: windowContentView,
                                    position: event.locationInWindow,
                                    isContainedBy: self)
        
        guard containerResult.isContain else {
            return
        }
        
        findSelectedViewCommonSteps(with: event,
                                    scrollNeedPosition: containerResult.convertedPosition)
    }
    #endif
    
    func findSelectedViewCommonSteps(with event: AGEEvent, scrollNeedPosition: CGPoint) {
        // Step 2, check per level scrollView contains this position
        var containedLevels = [(level: Int, convertedPosition: CGPoint)]()
        
        for (level, item) in levels {
            let view = item.viewType.view
            let scrollViewResult = check(superView: self,
                                     position: scrollNeedPosition,
                                     isContainedBy: view)
            
            if scrollViewResult.isContain {
                containedLevels.append((level,
                                        scrollViewResult.convertedPosition))
            }
        }
        
        containedLevels.sort { (num1, num2) -> Bool in
            return num1.level > num2.level
        }
        
        guard containedLevels.count > 0 else {
                return
        }

        var containedSubIndex: AGEIndex?
        var containedSubView: AGEView?
        
        for item in containedLevels {
            guard let subViewsOfScroll = viewsOfList(level: item.level),
                let scrollView = levels[item.level]?.viewType.view else {
                    continue
            }
            
            // Step 3, check per sub view of scrollView contains this position
            var subViewResult = PositionCheckResult(isContain: false,
                                                    convertedPosition: CGPoint.zero)
            
            for (index, view) in subViewsOfScroll.enumerated() {
                subViewResult = check(superView: scrollView,
                                      position: item.convertedPosition,
                                      isContainedBy: view)
                
                if subViewResult.isContain {
                    containedSubIndex = AGEIndex(level: item.level, item: index)
                    containedSubView = view
                    break
                }
            }
            
            if containedSubIndex != nil {
                break
            }
        }
        
        guard let subIndex = containedSubIndex,
            let subView = containedSubView else {
                return
        }

        AGELog.log("selected index, level: \(subIndex.level), item: \(subIndex.item)", type: .eventsTrain)
        
        delegate?.container(self, didSelected: subView, index: subIndex)
    }
    
    func check(superView: AGEView, position: CGPoint, isContainedBy subView: AGEView) -> PositionCheckResult {
        let convertedPoint = superView.convert(position, to: subView)
        let isContain = subView.bounds.contains(convertedPoint)
        AGELog.log("position, x:\(position.x), y: \(position.y)", type: .eventsTrain)
        AGELog.log("convertedPoint, x:\(convertedPoint.x), y: \(convertedPoint.y), isContain:\(isContain)", type: .eventsTrain)
        AGELog.log("subView frame, x:\(subView.frame.origin.x), y: \(subView.frame.origin.y), w: \(subView.frame.width), h: \(subView.frame.height)", type: .eventsTrain)
        AGELog.log(AGELog.separator, type: .eventsTrain)
        return PositionCheckResult(isContain: isContain,
                                   convertedPosition: convertedPoint)
    }
}

