/**
 *  the View is still responsible for handling events from the ViewModel.
 *  It’s important to remember the View isn’t responsible here for handling state - it keeps this in sync with the ViewModel.
 */


/**
 * ViewModel changes Model information into View information, passing commands from the View to the Model.
 */
// our main ViewModel
var ViewModel = function ( todos ) {
    var self = this;

    // map array of passed in todos to an observableArray of Todo objects
    self.todos = ko.observableArray(
        ko.utils.arrayMap(todos, function (todo) {
            return new Todo(todo.content, todo.done);
        }));

    // store the new todo value being entered
    self.current = ko.observable();

    // add a new todo, when enter key is pressed
    self.add = function (data, event) {
        var newTodo, current = self.current().trim();
        if (current) {
            newTodo = new Todo(current);
            self.todos.push(newTodo);
            self.current("");
        }
    };

    // remove a single todo
    self.remove = function (todo) {
        self.todos.remove(todo);
    };

    // remove all completed todos
    self.removeCompleted = function () {
        self.todos.remove(function (todo) {
            return todo.done();
        });
    };

    // writeable computed observable to handle marking all complete/incomplete
    self.allCompleted = ko.computed({

        // always return true/false based on the done flag of all todos
        read: function () {
            return !self.remainingCount();
        },

        // set all todos to the written value (true/false)
        write: function (newValue) {
            ko.utils.arrayForEach(self.todos(), function (todo) {
                //set even if value is the same, as subscribers are not notified in that case
                todo.done(newValue);
            });
        }
    });

    // edit an item
    self.editItem = function (item) {
        item.editing(true);
    };
}